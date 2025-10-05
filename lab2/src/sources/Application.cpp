#include "Application.hpp"
#include <memory>

namespace RA
{
    std::shared_ptr<Application> Application::Instance = nullptr;

    Application::Application()
    {
        _Camera = std::make_shared<Camera>();
    }

    void Application::Run()
    {
        // Open Window
        _Window = std::make_unique<Window>(1000, 800, "Računalna Animacija - Laboratorijska Vježba 2");

        // Load Assets
        Assets::LoadAssets();

        // Setup Renderer
        _Renderer = std::make_shared<Renderer>();

        // Start Application Loop
        _Loop();
    }

    void Application::_Loop()
    {
        float lastFrame = 0.0f;

        while (!_Window->ShouldClose())
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            float delta_time = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Input
            Input::ProcessCameraInput(_Window->GetNativeHandle(), delta_time, _Camera);

            // Render scene via renderer
            _Renderer->Render(_Camera);

            _Window->SwapBuffers();
            _Window->PollEvents();
        }
    }
}