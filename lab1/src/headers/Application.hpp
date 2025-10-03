#pragma once

// Local headers
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Renderer.hpp"

// External headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

namespace RA
{
    class Application
    {
    public:
        static std::shared_ptr<Application> Instance;

        Application(std::string mesh_file, std::string curve_file);

        void Run();
        glm::mat4 GetPerspectiveMatrix();

    private:
        // File paths
        const std::string _MeshFile;
        const std::string _CurveFile;

        // Core subsystems
        std::unique_ptr<Window> _window;
        std::unique_ptr<Renderer> _renderer;

        // Assets
        std::shared_ptr<Mesh> Mesh;
        std::shared_ptr<Shader> _ObjectShader;

        // Camera
        Transform Camera;

        // Mouse tracking
        double _lastMouseX = 0.0;
        double _lastMouseY = 0.0;
        bool _firstMouse = true;

        // Helpers
        void _LoadAssets();
        void _AppLoop();
        void _MoveCamera(float deltaTime);
    };
}