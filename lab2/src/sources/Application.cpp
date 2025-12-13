#include "Application.hpp"

namespace RA::Application
{
    std::shared_ptr<RA::Window> Window = nullptr;
    std::shared_ptr<RA::Camera> Camera = nullptr;
    std::shared_ptr<RA::ParticleSystem> CloudPS = nullptr;
    std::shared_ptr<RA::ParticleSystem> StarsPS = nullptr;
    std::shared_ptr<RA::ParticleSystem> SnowPS = nullptr;
}

void RA::Application::Initialize()
{
    Window = std::make_shared<RA::Window>(1000, 800, "2nd Laboratory Exercise");
    Camera = std::make_shared<RA::Camera>();

    CloudPS = std::make_shared<RA::ParticleSystem>(50);
    CloudPS->LoadTexture("assets/cloud.png");
    CloudPS->Properties.MaximumLifeLength = 16.f;
    CloudPS->Properties.MinimumLifeLength = 8.f;
    CloudPS->Properties.StartColor = glm::vec4(0.7, 0.7, 0.7, 0.8);
    CloudPS->Properties.EndColor = glm::vec4(0.5, 0.5, 0.5, 0.4);
    CloudPS->Properties.Frequency = 10.f;
    CloudPS->Properties.SourcePosition = glm::vec3(0.0, 10.0, 1.0);
    CloudPS->Properties.SourceSphereRadius = glm::vec3(5.0, 2.0, 2.0);
    CloudPS->Properties.MaximumStartSize = 18.f;
    CloudPS->Properties.MinimumStartSize = 10.f;
    CloudPS->Properties.Gravity = false;
    CloudPS->Properties.SizeFalloff = 0.0f;

    StarsPS = std::make_shared<RA::ParticleSystem>(100);
    StarsPS->LoadTexture("assets/star.png");
    StarsPS->Properties.MaximumLifeLength = 1000.f;
    StarsPS->Properties.MinimumLifeLength = 100.f;
    StarsPS->Properties.StartColor = glm::vec4(1.0, 1.0, 1.0, 0.8);
    StarsPS->Properties.EndColor = glm::vec4(1.0, 1.0, 1.0, 0.8);
    StarsPS->Properties.Frequency = 10.f;
    StarsPS->Properties.SourcePosition = glm::vec3(0.0, 50.0, 0.0);
    StarsPS->Properties.SourceSphereRadius = glm::vec3(100.0, 0.0, 100.0);
    StarsPS->Properties.MaximumStartSize = 2.f;
    StarsPS->Properties.Gravity = false;
    StarsPS->Properties.SizeFalloff = 0.1f;
    StarsPS->Properties.StartVelocityStrength = 0.f;

    SnowPS = std::make_shared<RA::ParticleSystem>(100); // more particles than clouds
    SnowPS->LoadTexture("assets/snow.png");
    SnowPS->Properties.MaximumLifeLength = 8.f;
    SnowPS->Properties.MinimumLifeLength = 4.f;
    SnowPS->Properties.StartColor = glm::vec4(1.0, 1.0, 1.0, 0.9);
    SnowPS->Properties.EndColor = glm::vec4(1.0, 1.0, 1.0, 0.5);
    SnowPS->Properties.Frequency = 10.f;
    SnowPS->Properties.SourcePosition = glm::vec3(0.0, 12.0, 1.0);
    SnowPS->Properties.SourceSphereRadius = glm::vec3(5.0, 0.5, 5.0);
    SnowPS->Properties.MaximumStartSize = 0.2f;
    SnowPS->Properties.MinimumStartSize = 0.1f;
    SnowPS->Properties.Gravity = true;
    SnowPS->Properties.StartVelocityStrength = 0.9f;
    SnowPS->Properties.SizeFalloff = 0.0f;
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
        Window->Clear(0.039, 0.039, 0.118, 1);

        // Calculate the delta_time
        float current_frame = static_cast<float>(glfwGetTime());
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Input: Camera Movement.
        InputMoveCamera(Window->GetNativeHandle(), delta_time, Camera);

        CloudPS->Update(delta_time);
        StarsPS->Update(delta_time);
        SnowPS->Update(delta_time);

        // Render all particle systems.
        StarsPS->Render(Camera, Window);
        CloudPS->Render(Camera, Window);
        SnowPS->Render(Camera, Window);

        Window->SwapBuffers();
        Window->PollEvents();
    }
}