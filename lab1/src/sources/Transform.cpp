#include "Transform.hpp"

namespace RA
{
    Transform::Transform()
        : _position(0.0f), _front(0.0f, 0.0f, 1.0f), _up(0.0f, 1.0f, 0.0f), _right(1.0f, 0.0f, 0.0f),
          _scale(1.0f), _dirty(true), _modelMatrix(1.0f)
    {
    }

    void Transform::_RecalculateModelMatrix()
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), _position);
        glm::mat4 rotation(1.0f);
        rotation[0] = glm::vec4(_right, 0.0f);
        rotation[1] = glm::vec4(_up, 0.0f);
        rotation[2] = glm::vec4(-_front, 0.0f);
        glm::mat4 scaling = glm::scale(glm::mat4(1.0f), _scale);

        _modelMatrix = translation * rotation * scaling;
        _dirty = false;
    }

    glm::mat4 Transform::GetModelMatrix()
    {
        if (_dirty)
            _RecalculateModelMatrix();
        return _modelMatrix;
    }

    // --- Movement & Rotation ---
    void Transform::Rotate(const glm::mat4 &rotation)
    {
        _front = glm::normalize(glm::vec3(rotation * glm::vec4(_front, 0.0f)));
        _up = glm::normalize(glm::vec3(rotation * glm::vec4(_up, 0.0f)));
        _right = glm::normalize(glm::cross(_front, _up));
        _dirty = true;
    }

    void Transform::Rotate(float x_offset, float y_offset, bool constrain_pitch, float sensitivity)
    {
        x_offset *= sensitivity;
        y_offset *= sensitivity;

        glm::mat4 yaw = glm::rotate(glm::mat4(1.0f), glm::radians(-x_offset), glm::vec3(0, 1, 0));
        glm::vec3 new_front = glm::vec3(yaw * glm::vec4(_front, 0.0f));
        glm::vec3 right = glm::normalize(glm::cross(new_front, glm::vec3(0, 1, 0)));
        glm::mat4 pitch = glm::rotate(glm::mat4(1.0f), glm::radians(y_offset), right);
        new_front = glm::vec3(pitch * glm::vec4(new_front, 0.0f));

        if (constrain_pitch)
        {
            float pitchAngle = glm::degrees(asin(glm::clamp(new_front.y, -1.0f, 1.0f)));
            if (pitchAngle > 89.0f || pitchAngle < -89.0f)
                return;
        }

        _front = glm::normalize(new_front);
        _right = glm::normalize(glm::cross(_front, glm::vec3(0, 1, 0)));
        _up = glm::normalize(glm::cross(_right, _front));

        _dirty = true;
    }

    void Transform::MoveG(const glm::vec3 &delta)
    {
        _position += delta;
        _dirty = true;
    }
    void Transform::MoveL(const glm::vec3 &delta)
    {
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
        _front = glm::normalize(front);
        _right = glm::normalize(glm::cross(_front, up));
        _up = glm::normalize(glm::cross(_right, _front));
        _dirty = true;
    }
    void Transform::SetScale(const glm::vec3 &scale)
    {
        _scale = scale;
        _dirty = true;
    }
}