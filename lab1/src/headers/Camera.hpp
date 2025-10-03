#pragma once
#include "Transform.hpp"

namespace RA
{
    class Camera : public Transform
    {
    private:
        glm::mat4 _viewMatrix;

        void _RecalculateViewMatrix();

    public:
        Camera(); // Constructor sets default orientation along -Z

        glm::mat4 GetViewMatrix();
    };
}