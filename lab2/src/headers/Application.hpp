#pragma once

// Local
#include "Camera.hpp"
#include "ParticleSystem.hpp"
#include "Window.hpp"
// Standard
#include <iostream>
#include <memory>
#include <vector>

namespace RA
{
    /// The namespace holding everything necessary for the application to run.
    namespace Application
    {
        /// Initializes vital components of the application, in order to be able to run it.
        void Initialize();

        /// Runs the application loop using vital components (Window, Camera, ParticleSystems)
        void Run();

        extern std::shared_ptr<RA::ParticleSystem> CloudPS;
        extern std::shared_ptr<RA::ParticleSystem> StarsPS;
        extern std::shared_ptr<RA::ParticleSystem> SnowPS;

        /// Window component of the Application.
        extern std::shared_ptr<RA::Window> Window;

        /// Camera component of the Application.
        extern std::shared_ptr<RA::Camera> Camera;
    };
};