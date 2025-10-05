#pragma once

// Local headers
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "Input.hpp"

// Standard headers

// External headers

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
        std::shared_ptr<Window> _Window;     ///< Main application window
        std::shared_ptr<Renderer> _Renderer; ///< Renderer for drawing
        std::shared_ptr<Camera> _Camera;     ///< Main camera for the scene

        /// @brief Main application loop
        void _Loop();
    };
}