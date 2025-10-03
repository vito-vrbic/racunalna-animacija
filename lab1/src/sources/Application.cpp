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
        Camera = Transform();
        Camera.SetOrientation(glm::vec3(0.f, 0.f, -1.f), Camera.GetUp());
    }

    void Application::Run()
    {
        // Create subsystems
        _window = std::make_unique<Window>(1000, 800, "Laboratorijska Vježba 1");
        _renderer = std::make_unique<Renderer>();

        _renderer->SetWireframe(true);

        _LoadAssets();
        _AppLoop();
    }

    glm::mat4 Application::GetPerspectiveMatrix()
    {
        int width, height;
        glfwGetFramebufferSize(_window->GetNativeHandle(), &width, &height);

        float aspect = (height > 0) ? static_cast<float>(width) / height : 1.0f;

        return glm::perspective(glm::radians(45.f), aspect, 0.1f, 100.f);
    }

    void Application::_LoadAssets()
    {
        Mesh = Mesh::LoadMesh(_MeshFile);
        _ObjectShader = Shader::LoadShader("object");
    }

    void Application::_AppLoop()
    {
        float lastFrame = 0.0f;

        while (!_window->ShouldClose())
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Input
            _MoveCamera(deltaTime);

            // Render
            _renderer->Clear();
            Mesh->Render(_ObjectShader, Camera.GetViewMatrix(), GetPerspectiveMatrix());

            _window->SwapBuffers();
            _window->PollEvents();
        }
    }

    void Application::_MoveCamera(float deltaTime)
    {
        float cameraSpeed = 1.5f * deltaTime;

        if (glfwGetKey(_window->GetNativeHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cameraSpeed *= 3.0f;
        else
            cameraSpeed *= 0.5f;

        // Keyboard input
        if (glfwGetKey(_window->GetNativeHandle(), GLFW_KEY_W) == GLFW_PRESS)
            Camera.MoveL(glm::vec3(0.0f, 0.0f, -cameraSpeed));
        if (glfwGetKey(_window->GetNativeHandle(), GLFW_KEY_S) == GLFW_PRESS)
            Camera.MoveL(glm::vec3(0.0f, 0.0f, cameraSpeed));
        if (glfwGetKey(_window->GetNativeHandle(), GLFW_KEY_A) == GLFW_PRESS)
            Camera.MoveL(glm::vec3(-cameraSpeed, 0.0f, 0.0f));
        if (glfwGetKey(_window->GetNativeHandle(), GLFW_KEY_D) == GLFW_PRESS)
            Camera.MoveL(glm::vec3(cameraSpeed, 0.0f, 0.0f));
        if (glfwGetKey(_window->GetNativeHandle(), GLFW_KEY_Q) == GLFW_PRESS)
            Camera.MoveL(glm::vec3(0.0f, -cameraSpeed, 0.0f));
        if (glfwGetKey(_window->GetNativeHandle(), GLFW_KEY_E) == GLFW_PRESS)
            Camera.MoveL(glm::vec3(0.0f, cameraSpeed, 0.0f));

        // Mouse rotation (right button)
        if (glfwGetMouseButton(_window->GetNativeHandle(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(_window->GetNativeHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double mouseX, mouseY;
            glfwGetCursorPos(_window->GetNativeHandle(), &mouseX, &mouseY);

            if (_firstMouse)
            {
                _lastMouseX = mouseX;
                _lastMouseY = mouseY;
                _firstMouse = false;
            }

            float xoffset = static_cast<float>(mouseX - _lastMouseX);
            float yoffset = static_cast<float>(_lastMouseY - mouseY);

            _lastMouseX = mouseX;
            _lastMouseY = mouseY;

            const float sensitivity = 0.5f;
            Camera.Rotate(xoffset, yoffset, true, sensitivity);
        }
        else
        {
            glfwSetInputMode(_window->GetNativeHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            _firstMouse = true;
        }
    }
}