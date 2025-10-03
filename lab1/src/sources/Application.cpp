#include "Application.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace RA
{
    std::shared_ptr<Application> Application::Instance = nullptr;

    Application::Application()
    {
        _Camera = Camera();
    }

    void Application::Run()
    {
        // Open Window
        _Window = std::make_unique<Window>(1000, 800, "Računalna Animacija - Laboratorijska Vježba 1");

        // Setup of Rendering
        _Renderer = std::make_unique<Renderer>();
        _Renderer->SetWireframe(true);

        // Load Assets
        _Assets = Assets();
        _Assets.LoadAssets();

        // Start Application Loop
        _Loop();
    }

    void Application::_Loop()
    {
        float lastFrame = 0.0f;

        while (!_Window->ShouldClose())
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Input
            Input::ProcessCameraInput(_Window->GetNativeHandle(), deltaTime, _Camera);
            Input::ProcessBSplineFollow(_Window->GetNativeHandle(), deltaTime, _Assets.ObjectMesh, _Assets.ObjectTangent, _Assets.BSplineCurve);

            // Render
            _Renderer->Clear();

            _Assets.ObjectMesh->Render(_Assets.ObjectShader, _Camera.GetViewMatrix(), _Window->GetPerspectiveMatrix());
            _Assets.BSplineCurve->Render(_Assets.PolylineShader, _Camera.GetViewMatrix(), _Window->GetPerspectiveMatrix());
            _Assets.ObjectTangent->Render(_Assets.PolylineShader, _Camera.GetViewMatrix(), _Window->GetPerspectiveMatrix());

            _Window->SwapBuffers();
            _Window->PollEvents();
        }
    }
}