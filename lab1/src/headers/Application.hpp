#pragma once

// Local Headers
#include "Mesh.hpp"
#include "Shader.hpp"
// Standard Headers
#include <iostream>
#include <string>
#include <memory>
// External Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace RA
{
    class Application
    {
    public:
        static std::shared_ptr<Application> Instance;
        std::shared_ptr<Mesh> Mesh;
        Transform Camera;
        void Run();

        Application(std::string mesh_file, std::string curve_file);

        glm::mat4 GetPerspectiveMatrix();

    private:
        const std::string _MeshFile,
            _CurveFile;
        GLFWwindow *_Window;

        std::shared_ptr<Shader> _ObjectShader;

        void _InitConfigGLFW();
        void _CreateWindow();
        void _InitConfigGLADOpenGL();
        void _LoadAssets();
        void _AppLoop();
        void _CleanUp();

        void _MoveCamera(float deltaTime);

        double _lastMouseX = 0.0;
        double _lastMouseY = 0.0;
        bool _firstMouse = true;

        static void _FramebufferSizeCallback(GLFWwindow *window, int width, int height);
    };
};