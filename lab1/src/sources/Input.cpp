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

    // Compute position, tangent, normal on B-spline
    glm::vec3 position = spline->GetPoint(current_segment, current_t);
    glm::vec3 tangent = spline->GetTangent(current_segment, current_t);
    glm::vec3 normal = spline->GetNormal(current_segment, current_t);
    glm::vec3 binormal = glm::cross(tangent, normal);

    glm::vec3 current_front(tangent);

    _PrintAxisDifference(current_front, delta_time);

    // Update mesh
    if (mesh)
    {
        mesh->SetPosition(position);
        mesh->SetOrientation(tangent, normal, binormal);
    }

    if (front_vec)
    {
        front_vec->SetPosition(position);
        front_vec->SetOrientation(tangent, normal, binormal);
    }
}

void RA::Input::_ComputeRotationFromTo(const glm::vec3 &s, const glm::vec3 &e, glm::vec3 &out_axis, float &out_angle)
{
    glm::vec3 start = glm::normalize(s);
    glm::vec3 end = glm::normalize(e);

    // Rotation axis = s cross e
    out_axis = glm::cross(start, end);

    float axisLength = glm::length(out_axis);

    if (axisLength < 1e-6f)
    {
        // No rotation if vectors are parallel.
        out_axis = glm::vec3(0, 1, 0);
        out_angle = 0.0f;
        return;
    }

    out_axis = glm::normalize(out_axis);

    // Rotation = arccos(s dot e)
    float cosTheta = glm::clamp(glm::dot(start, end), -1.0f, 1.0f);
    out_angle = glm::degrees(acos(cosTheta));
}

void RA::Input::_PrintAxisDifference(const glm::vec3 &current_front, float delta_time)
{
    static float time_accumulator = 0.0f;
    static float print_interval = 0.1f; // seconds
    static glm::vec3 original_front;
    static bool has_original = false;

    // Initialize on first call
    if (!has_original)
    {
        original_front = current_front;
        has_original = true;
        return;
    }

    // Accumulate time
    time_accumulator += delta_time;

    // Every few seconds, compute and print difference
    if (time_accumulator >= print_interval)
    {
        glm::vec3 axis;
        float angle;
        _ComputeRotationFromTo(original_front, current_front, axis, angle);

        std::cout << "[DEBUG]: Rotation Difference After " << print_interval << "s" << std::endl;

        std::cout << "From (" << original_front.x << ", " << original_front.y << ", " << original_front.z << ")" << "to (" << current_front.x << ", " << current_front.y << ", " << current_front.z << ")" << std::endl;

        std::cout << "Axis: (" << axis.x << ", " << axis.y << ", " << axis.z << ") " << std::endl;

        std::cout << "Angle diff: " << angle << std::endl;

        std::cout << std::endl;

        // Reset for next interval
        time_accumulator = 0.0f;
        original_front = current_front;
    }
}