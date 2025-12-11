#include "Window.hpp"

RA::Window::Window(int width, int height, const std::string &title)
{
    if (!glfwInit())
    {
        std::cerr << "[ERROR]: Failed to initialize GLFW." << std::endl;
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    _InitOpenGL();
}

RA::Window::~Window()
{
    glfwDestroyWindow(_Window);
    glfwTerminate();
}

bool RA::Window::ShouldClose() const
{
    return glfwWindowShouldClose(_Window);
}

void RA::Window::PollEvents() const
{
    glfwPollEvents();
}

void RA::Window::SwapBuffers() const
{
    glfwSwapBuffers(_Window);
}

void RA::Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void RA::Window::_InitOpenGL()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void RA::Window::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

glm::mat4 RA::Window::GetPerspectiveMatrix(float fov, float nearPlane, float farPlane) const
{
    int width, height;
    glfwGetFramebufferSize(_Window, &width, &height);

    float aspect = (height > 0) ? static_cast<float>(width) / height : 1.0f;

    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}