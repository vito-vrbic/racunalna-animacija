#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace RA
{
    Camera::Camera() : Transform()
    {
        // Default orientation: front = -Z, up = Y
        SetOrientation(glm::vec3(0.f, 0.f, -1.f), GetUp());

        // Initialize view matrix
        _viewMatrix = glm::lookAt(_position, _position + _front, _up);
    }

    void Camera::_RecalculateViewMatrix()
    {
        _viewMatrix = glm::lookAt(_position, _position + _front, _up);
    }

    glm::mat4 Camera::GetViewMatrix()
    {
        if (_dirty) // Only recalc if Transform changed
        {
            _RecalculateViewMatrix();
            _dirty = false; // Reset dirty flag
        }
        return _viewMatrix;
    }
}