#pragma once

#include <GLFW/glfw3.h>
#include "Transform.hpp"
namespace RA
{
    class Input
    {
    public:
        static void ProcessCameraInput(GLFWwindow *window, float deltaTime, RA::Transform &camera);

    private:
        static inline double _LastMouseX = 0.0;
        static inline double _LastMouseY = 0.0;
        static inline bool _FirstMouse = true;
    };
};
