#pragma once

// Local headers
#include "Transform.hpp"

namespace RA
{
    class Camera : public Transform
    {
    public:
        Camera();

        glm::mat4 GetViewMatrix();

    private:
        glm::mat4 _view_matrix;

        void _RecalculateViewMatrix();
    };
}