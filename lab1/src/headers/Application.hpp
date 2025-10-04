#pragma once

// Local headers
#include "Assets.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

// Standard headers
#include <iostream>
#include <memory>
#include <string>

// External headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace RA
{
    class Application
    {
    public:
        /// @brief Singleton instance of the application
        static std::shared_ptr<Application> Instance;

        /// @brief Constructor
        Application();

        /// @brief Start the application loop
        void Run();

    private:
        std::unique_ptr<Window> _Window;     ///< Main application window
        std::unique_ptr<Renderer> _Renderer; ///< Renderer for drawing

        Assets _Assets; ///< Loaded assets (meshes, shaders, textures, etc.)

        Camera _Camera; ///< Main camera for the scene

        /// @brief Main application loop
        void _Loop();
    };
}