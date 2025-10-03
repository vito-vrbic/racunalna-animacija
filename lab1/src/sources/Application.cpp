#include "Application.hpp"

std::shared_ptr<RA::Application> RA::Application::Instance = nullptr;

void RA::Application::Run()
{
    _InitConfigGLFW();

    _CreateWindow();

    _InitConfigGLADOpenGL();

    _LoadAssets();

    _AppLoop();

    _CleanUp();
}

RA::Application::Application(std::string mesh_file, std::string curve_file) : _MeshFile(mesh_file), _CurveFile(curve_file)
{
    Camera = Transform();
    Camera.SetOrientation(glm::vec3(0.f, 0.f, -1.f), Camera.GetUp());
}

glm::mat4 RA::Application::GetPerspectiveMatrix()
{
    int width, height;
    glfwGetFramebufferSize(_Window, &width, &height);

    float aspect = (height > 0) ? static_cast<float>(width) / height : 1.0f;

    return glm::perspective(glm::radians(45.f), aspect, 0.1f, 100.f);
}

void RA::Application::_InitConfigGLFW()
{
    if (!glfwInit())
    {
        std::cout << "[ERROR]: Failed to initialize GLFW." << std::endl;
        exit(1);
    }

    // Configure GLFW (optional)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void RA::Application::_CreateWindow()
{
    _Window = glfwCreateWindow(1000, 800, "Laboratorijska Vježba 1", nullptr, nullptr);
    if (!_Window)
    {
        std::cout << "[ERROR]: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(_Window);
    // Set framebuffer size callback for window resizing
    glfwSetFramebufferSizeCallback(_Window, RA::Application::_FramebufferSizeCallback);
}

void RA::Application::_InitConfigGLADOpenGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "[ERROR]: Failed to initialize GLAD." << std::endl;
        return;
    }

    // Print GL version (nice sanity check)
    std::cout << "[DEBUG]: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable face culling
    glEnable(GL_CULL_FACE);

    // Specify that back faces should be culled
    glCullFace(GL_BACK);

    // Optional: define the front face winding order (counter-clockwise is default)
    glFrontFace(GL_CW);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RA::Application::_LoadAssets()
{
    this->Mesh = Mesh::LoadMesh(_MeshFile);
    // Load Curve
    // Load Shaders
    this->_ObjectShader = Shader::LoadShader("object");
}

void RA::Application::_AppLoop()
{
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(_Window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle input
        _MoveCamera(deltaTime);

        // --- Rendering commands here ---
        glClearColor(1.f, 1.f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Mesh->Render(_ObjectShader, Camera.GetViewMatrix(), GetPerspectiveMatrix());

        // Swap front and back buffers
        glfwSwapBuffers(_Window);

        // Poll for and process events
        glfwPollEvents();
    }
}

void RA::Application::_CleanUp()
{
    glfwDestroyWindow(_Window);
    glfwTerminate();
}

void RA::Application::_MoveCamera(float deltaTime)
{
    // Base camera speed
    float cameraSpeed = 1.5f * deltaTime; // units per second

    // Shift key modifies speed
    if (glfwGetKey(_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 3.0f; // go faster
    else
        cameraSpeed *= 0.5f; // go slower when not holding shift

    // Keyboard movement
    if (glfwGetKey(_Window, GLFW_KEY_W) == GLFW_PRESS)
        Camera.MoveL(glm::vec3(0.0f, 0.0f, -cameraSpeed)); // forward
    if (glfwGetKey(_Window, GLFW_KEY_S) == GLFW_PRESS)
        Camera.MoveL(glm::vec3(0.0f, 0.0f, cameraSpeed)); // backward
    if (glfwGetKey(_Window, GLFW_KEY_A) == GLFW_PRESS)
        Camera.MoveL(glm::vec3(-cameraSpeed, 0.0f, 0.0f)); // left
    if (glfwGetKey(_Window, GLFW_KEY_D) == GLFW_PRESS)
        Camera.MoveL(glm::vec3(cameraSpeed, 0.0f, 0.0f)); // right
    if (glfwGetKey(_Window, GLFW_KEY_Q) == GLFW_PRESS)
        Camera.MoveL(glm::vec3(0.0f, -cameraSpeed, 0.0f)); // down
    if (glfwGetKey(_Window, GLFW_KEY_E) == GLFW_PRESS)
        Camera.MoveL(glm::vec3(0.0f, cameraSpeed, 0.0f)); // up

    // Mouse movement (while right button is pressed)
    if (glfwGetMouseButton(_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        // Hide cursor
        glfwSetInputMode(_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double mouseX, mouseY;
        glfwGetCursorPos(_Window, &mouseX, &mouseY);

        if (_firstMouse)
        {
            _lastMouseX = mouseX;
            _lastMouseY = mouseY;
            _firstMouse = false;
        }

        float xoffset = static_cast<float>(mouseX - _lastMouseX);
        float yoffset = static_cast<float>(_lastMouseY - mouseY); // reversed

        _lastMouseX = mouseX;
        _lastMouseY = mouseY;

        const float sensitivity = 0.5f; // tweak to preference
        Camera.Rotate(xoffset, yoffset, true, sensitivity);
    }
    else
    {
        // Show cursor when not dragging
        glfwSetInputMode(_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        _firstMouse = true;
    }
}

void RA::Application::_FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
