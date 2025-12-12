#pragma once

// Local
#include "Camera.hpp"
#include "ParticleSystem.hpp"
#include "Window.hpp"
// Standard
#include <iostream>
#include <memory>

namespace RA
{
    namespace Application
    {
        /// Initializes all the important stuff about the application.
        void Initialize();

        /// Runs the application loop.
        void Run();

        extern std::shared_ptr<ParticleSystem> TestPS;

        extern std::shared_ptr<RA::Window> Window;
        extern std::shared_ptr<RA::Camera> Camera;
    };
};