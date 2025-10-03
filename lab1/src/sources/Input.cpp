#include "Input.hpp"
#include <iostream>

void RA::Input::ProcessCameraInput(GLFWwindow *window, float delta_time, RA::Transform &camera)
{
    static double last_mouse_x = 0.0;
    static double last_mouse_y = 0.0;
    static bool first_mouse = true;

    float cameraSpeed = 1.5f * delta_time;

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
        camera.Rotate(xoffset, yoffset, true, sensitivity);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        first_mouse = true;
    }
}

void RA::Input::ProcessBSplineFollow(GLFWwindow *window, float delta_time, std::shared_ptr<Mesh> mesh, std::shared_ptr<Polyline> front_vec, std::shared_ptr<BSpline> spline)
{
    static const float speed = 1.0f;

    static bool follow_spline = false;
    static bool space_pressed_last_frame = false;
    static float current_t = 0;
    static int current_segment = 0;

    bool space_pressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

    if (space_pressed && !space_pressed_last_frame)
    {
        follow_spline = !follow_spline;
        current_t = 0.0f;
        current_segment = 0;
    }
    space_pressed_last_frame = space_pressed;

    if (!follow_spline || spline->GetControlPoints().size() < 4)
        return;

    // Advance current_t based on speed and delta_time
    current_t += speed * delta_time;

    size_t num_segments = spline->GetControlPoints().size() - 3;

    // Loop along the spline
    while (current_t >= 1.0f)
    {
        current_t -= 1.0f;
        current_segment++;

        if (current_segment >= num_segments)
            current_segment = 0; // loop to start
    }

    // Compute position on B-spline
    glm::vec3 position = spline->GetPoint(current_segment, current_t);
    glm::vec3 front = spline->GetTangent(current_segment, current_t);

    // Update mesh
    if (mesh)
    {
        mesh->SetPosition(position);
        mesh->SetOrientation(front, glm::vec3(0.f, 1.f, 0.f));
    }

    if (front_vec)
    {
        front_vec->SetPosition(position);
        front_vec->SetOrientation(front, glm::vec3(0.f, 1.f, 0.f));
    }
}
