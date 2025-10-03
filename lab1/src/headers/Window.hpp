#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>

class Window
{
public:
    Window(int width, int height, const std::string &title);
    ~Window();

    bool ShouldClose() const;
    void PollEvents() const;
    void SwapBuffers() const;

    GLFWwindow *GetNativeHandle() const { return _Window; }

    // New: Perspective matrix from window size
    glm::mat4 GetPerspectiveMatrix(float fov = 45.f, float nearPlane = 0.1f, float farPlane = 100.f) const;

private:
    GLFWwindow *_Window;

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
};