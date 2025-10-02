// Local Headers
#include "Transform.hpp"

namespace IRG
{
    void Transform::CleanTransform()
    {
        _dirty = false;
    }

    bool Transform::GetDirty() const
    {
        return _dirty;
    }

    Transform::Transform() : _position(0.0f, 0.0f, 0.0f),

                             _front(0.0f, 0.0f, 1.0f),
                             _up(0.0f, 1.0f, 0.0f),
                             _right(1.0f, 0.0f, 0.0f),

                             _scale(1.0f, 1.0f, 1.0f),
                             _dirty(true)
    {
    }

    void Transform::Rotate(const glm::mat4 &rotation)
    {
        // Rotate the front and up vectors using the rotation matrix.
        glm::vec4 rotated_front = rotation * glm::vec4(_front, 0.0f);
        glm::vec4 rotated_up = rotation * glm::vec4(_up, 0.0f);
        _front = glm::normalize(glm::vec3(rotated_front));
        _up = glm::normalize(glm::vec3(rotated_up));

        // Recalculate the right vector to ensure an orthogonal basis.
        _right = glm::normalize(glm::cross(_front, _up));
        _dirty = true;
    }

    void Transform::Rotate(float x_offset, float y_offset, bool constrain_pitch, float sensitivity)
    {
        x_offset *= sensitivity;
        y_offset *= sensitivity;

        // Rotate around world up (yaw)
        glm::mat4 yaw_rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-x_offset), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 new_front = glm::vec3(yaw_rotation * glm::vec4(_front, 0.0f));

        // Right vector from updated yawed front
        glm::vec3 right = glm::normalize(glm::cross(new_front, glm::vec3(0.0f, 1.0f, 0.0f)));

        // Pitch rotation around local right axis
        glm::mat4 pitch_rotation = glm::rotate(glm::mat4(1.0f), glm::radians(y_offset), right);
        new_front = glm::vec3(pitch_rotation * glm::vec4(new_front, 0.0f));

        // Optional: Constrain pitch if needed
        if (constrain_pitch)
        {
            float pitch = glm::degrees(asin(glm::clamp(new_front.y, -1.0f, 1.0f)));
            const float pitch_limit = 89.0f;
            if (pitch > pitch_limit || pitch < -pitch_limit)
                return; // Skip applying the rotation if it exceeds pitch bounds
        }

        // Apply and update orientation basis
        _front = glm::normalize(new_front);
        _right = glm::normalize(glm::cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
        _up = glm::normalize(glm::cross(_right, _front));

        _dirty = true;
    }

    void Transform::MoveG(const glm::vec3 &delta)
    {
        // Global movement: directly adjust the position.
        _position += delta;
        _dirty = true;
    }

    void Transform::MoveL(const glm::vec3 &delta)
    {
        // Local movement: combine movement in the direction of local axes.
        _position += _right * delta.x + _up * delta.y - _front * delta.z;
        _dirty = true;
    }

    void Transform::SetPosition(const glm::vec3 &position)
    {
        _position = position;
        _dirty = true;
    }

    void Transform::SetOrientation(const glm::vec3 &front, const glm::vec3 &up)
    {
        // Set and normalize the front vector.
        _front = glm::normalize(front);

        // Compute the right vector from the front and provided up vector.
        _right = glm::normalize(glm::cross(_front, up));

        // Recalculate up to ensure it is perpendicular.
        _up = glm::normalize(glm::cross(_right, _front));

        _dirty = true;
    }

    void Transform::SetScale(const glm::vec3 &scale)
    {
        _scale = scale;
        _dirty = true;
    }
}
