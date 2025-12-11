#include "Camera.hpp"

RA::Camera::Camera()
    : _position(0.f, 0.f, 0.f),
      _front(0.f, 0.f, -1.f),
      _up(0.f, 1.f, 0.f),
      _yaw(-90.f),
      _pitch(0.f),
      _dirty(true)
{
    RecalculateVectors();
}

glm::mat4 RA::Camera::GetViewMatrix()
{
    if (_dirty)
    {
        _view = glm::lookAt(_position, _position + _front, _up);
        _dirty = false;
    }
    return _view;
}

void RA::Camera::Move(const glm::vec3 &deltaWorld)
{
    _position += deltaWorld;
    _dirty = true;
}

void RA::Camera::MoveLocal(const glm::vec3 &deltaLocal)
{
    _position += right * deltaLocal.x;
    _position += _up * deltaLocal.y;
    _position += _front * deltaLocal.z;
    _dirty = true;
}

void RA::Camera::Rotate(float yawOffset, float pitchOffset, float sensitivity, bool limitPitch)
{
    yawOffset *= sensitivity;
    pitchOffset *= sensitivity;

    _yaw += yawOffset;
    _pitch += pitchOffset;

    if (limitPitch)
        _pitch = glm::clamp(_pitch, -89.f, 89.f);

    RecalculateVectors();
    _dirty = true;
}

void RA::Camera::SetPosition(const glm::vec3 &p)
{
    _position = p;
    _dirty = true;
}

void RA::Camera::SetOrientation(const glm::vec3 &newFront, const glm::vec3 &worldUp)
{
    _front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(_front, worldUp));
    _up = glm::normalize(glm::cross(right, _front));

    // Recover yaw/pitch from direction
    _yaw = glm::degrees(atan2(_front.z, _front.x)) - 90.f;
    _pitch = glm::degrees(asin(_front.y));

    _dirty = true;
}

void RA::Camera::RecalculateVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    direction.y = sin(glm::radians(_pitch));
    direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

    _front = glm::normalize(direction);
    right = glm::normalize(glm::cross(_front, glm::vec3(0.f, 1.f, 0.f)));
    _up = glm::normalize(glm::cross(right, _front));
}