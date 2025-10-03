#include "Input.hpp"
#include <iostream>

void RA::Input::ProcessCameraInput(GLFWwindow *window, float deltaTime, RA::Transform &camera)
{
    float cameraSpeed = 1.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 3.0f;
    else
        cameraSpeed *= 0.5f;

    // Keyboard input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.MoveL(glm::vec3(0.0f, 0.0f, -cameraSpeed));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.MoveL(glm::vec3(0.0f, 0.0f, cameraSpeed));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.MoveL(glm::vec3(-cameraSpeed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.MoveL(glm::vec3(cameraSpeed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.MoveL(glm::vec3(0.0f, -cameraSpeed, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.MoveL(glm::vec3(0.0f, cameraSpeed, 0.0f));

    // Mouse input (right button)
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        if (_FirstMouse)
        {
            _LastMouseX = mouseX;
            _LastMouseY = mouseY;
            _FirstMouse = false;
        }

        float xoffset = static_cast<float>(mouseX - _LastMouseX);
        float yoffset = static_cast<float>(_LastMouseY - mouseY);

        _LastMouseX = mouseX;
        _LastMouseY = mouseY;

        const float sensitivity = 0.5f;
        camera.Rotate(xoffset, yoffset, true, sensitivity);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        _FirstMouse = true;
    }
}