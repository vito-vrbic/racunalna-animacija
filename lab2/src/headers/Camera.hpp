#pragma once

// Local
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RA
{
    class Camera
    {
    public:
        Camera();

        glm::mat4 GetViewMatrix();
        glm::vec3 GetPosition() const { return _position; }
        glm::vec3 GetFront() const { return _front; }
        glm::vec3 GetUp() const { return _up; }
        glm::vec3 GetRight() const { return right; }

        void Move(const glm::vec3 &deltaWorld);
        void MoveLocal(const glm::vec3 &deltaLocal);

        void Rotate(float yawOffset, float pitchOffset, float sensitivity = 0.1f, bool limitPitch = true);

        void SetPosition(const glm::vec3 &p);
        void SetOrientation(const glm::vec3 &newFront, const glm::vec3 &worldUp = glm::vec3(0, 1, 0));

    private:
        void RecalculateVectors();

        glm::vec3 _position;
        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 right;

        float _yaw;
        float _pitch;

        bool _dirty;
        glm::mat4 _view;
    };
};