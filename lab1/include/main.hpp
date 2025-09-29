#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Callback to adjust viewport on window resize
inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process input (close window on ESC)
inline void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}