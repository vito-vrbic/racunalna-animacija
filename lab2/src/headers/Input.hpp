#pragma once

// Local headers
#include "Transform.hpp"
#include "Camera.hpp"

// Standard header
#include <memory>

// External headers
#include <GLFW/glfw3.h>

namespace RA
{
    class Input
    {
    public:
        // Camera movement
        static void ProcessCameraInput(GLFWwindow *window, float delta_time, std::shared_ptr<Camera> &camera);

    private:
    };
}