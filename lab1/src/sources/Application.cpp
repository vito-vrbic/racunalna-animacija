#include "Application.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace RA
{
    std::shared_ptr<Application> Application::Instance = nullptr;

    Application::Application(std::string mesh_file, std::string curve_file)
        : _MeshFile(mesh_file), _CurveFile(curve_file)
    {
        _Camera = Camera();
    }

    void Application::Run()
    {
        // Create subsystems
        _Window = std::make_unique<Window>(1000, 800, "Laboratorijska Vježba 1");
        _Renderer = std::make_unique<Renderer>();

        _Renderer->SetWireframe(true);

        _LoadAssets();
        _AppLoop();
    }

    void Application::_LoadAssets()
    {
        _ObjectMesh = Mesh::LoadMesh(_MeshFile);
        _ObjectShader = Shader::LoadShader("object");
    }

    void Application::_AppLoop()
    {
        float lastFrame = 0.0f;

        while (!_Window->ShouldClose())
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Input
            Input::ProcessInput(_Window->GetNativeHandle(), deltaTime, _Camera);

            // Render
            _Renderer->Clear();
            _ObjectMesh->Render(_ObjectShader, _Camera.GetViewMatrix(), _Window->GetPerspectiveMatrix());

            _Window->SwapBuffers();
            _Window->PollEvents();
        }
    }
}