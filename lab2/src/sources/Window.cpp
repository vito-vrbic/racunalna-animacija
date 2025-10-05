#include "Window.hpp"

Window::Window(int width, int height, const std::string &title)
{
    if (!glfwInit())
    {
        std::cerr << "[ERROR]: Failed to initialize GLFW." << std::endl;
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!_Window)
    {
        std::cerr << "[ERROR]: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(_Window);
    glfwSetFramebufferSizeCallback(_Window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "[ERROR]: Failed to initialize GLAD." << std::endl;
        exit(1);
    }

    std::cout << "[DEBUG]: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window()
{
    glfwDestroyWindow(_Window);
    glfwTerminate();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_Window);
}

void Window::PollEvents() const
{
    glfwPollEvents();
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(_Window);
}

void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

glm::mat4 Window::GetPerspectiveMatrix(float fov, float nearPlane, float farPlane) const
{
    int width, height;
    glfwGetFramebufferSize(_Window, &width, &height);

    float aspect = (height > 0) ? static_cast<float>(width) / height : 1.0f;

    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}