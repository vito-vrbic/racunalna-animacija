#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

    GLFWwindow *GetNativeHandle() const { return _window; }

private:
    GLFWwindow *_window;

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
};