#pragma once

// Standard
#include <iostream>
#include <string>
// External
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace RA
{
    class Window
    {
    public:
        /// Initializes a window at a width, height, and with a title.
        Window(int width, int height, const std::string &title);
        ~Window();

        /// Returns true if the window needs to close.
        bool ShouldClose() const;

        /// Polls all the events of the window.
        void PollEvents() const;

        /// Swaps the graphics buffers.
        void SwapBuffers() const;

        /// Returns the native handle of type GLFWwindow.
        GLFWwindow *GetNativeHandle() const { return _Window; }

        /// Gets the window's perspective matrix.
        glm::mat4 GetPerspectiveMatrix(float fov = 45.f, float nearPlane = 0.1f, float farPlane = 100.f) const;

        /// Clears the buffer.
        void Clear(float r, float g, float b, float a);

    private:
        GLFWwindow *_Window;

        /// The callback for whenever the window is resized.
        static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);

        /// Initializes OpenGL.
        static void _InitOpenGL();
    };
};