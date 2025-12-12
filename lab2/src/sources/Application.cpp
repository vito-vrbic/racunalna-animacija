#include "Application.hpp"

namespace RA::Application
{
    std::shared_ptr<RA::Window> Window = nullptr;
    std::shared_ptr<RA::Camera> Camera = nullptr;
    std::shared_ptr<RA::ParticleSystem> TestPS = nullptr;
}

void RA::Application::Initialize()
{
    Application::Window = std::make_shared<RA::Window>(1000, 800, "2nd Laboratory Exercise");
    Application::Camera = std::make_shared<RA::Camera>();

    Application::TestPS = std::make_shared<RA::ParticleSystem>(1000);
}

void InputMoveCamera(GLFWwindow *window, float delta_time, std::shared_ptr<RA::Camera> &camera)
{
    static double last_mouse_x = 0.0;
    static double last_mouse_y = 0.0;
    static bool first_mouse = true;

    float cameraSpeed = 1.5f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 3.0f;
    else
        cameraSpeed *= 0.5f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->MoveLocal(glm::vec3(0.0f, 0.0f, cameraSpeed));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->MoveLocal(glm::vec3(0.0f, 0.0f, -cameraSpeed));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->MoveLocal(glm::vec3(-cameraSpeed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->MoveLocal(glm::vec3(cameraSpeed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->MoveLocal(glm::vec3(0.0f, -cameraSpeed, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->MoveLocal(glm::vec3(0.0f, cameraSpeed, 0.0f));

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        if (first_mouse)
        {
            last_mouse_x = mouseX;
            last_mouse_y = mouseY;
            first_mouse = false;
        }

        float xoffset = static_cast<float>(mouseX - last_mouse_x);
        float yoffset = static_cast<float>(last_mouse_y - mouseY);

        last_mouse_x = mouseX;
        last_mouse_y = mouseY;

        const float sensitivity = 0.5f;
        camera->Rotate(xoffset, yoffset, true, sensitivity);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        first_mouse = true;
    }
}

void RA::Application::Run()
{
    float last_frame = 0.0f;

    while (!Window->ShouldClose())
    {
        Window->Clear(0, 0, 0, 1);

        // Calculate the delta_time
        float current_frame = static_cast<float>(glfwGetTime());
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Input: Camera Movement.
        InputMoveCamera(Window->GetNativeHandle(), delta_time, Camera);

        TestPS->Update(delta_time, 100);

        // Render all particle systems.
        TestPS->Render(Camera, Window);

        Window->SwapBuffers();
        Window->PollEvents();
    }
}