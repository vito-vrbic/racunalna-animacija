#pragma once

// Local headers
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "Camera.hpp"

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

    private:
        // File paths
        const std::string _MeshFile;
        const std::string _CurveFile;

        // Core subsystems
        std::unique_ptr<Window> _Window;
        std::unique_ptr<Renderer> _Renderer;

        // Assets
        std::shared_ptr<Mesh> _ObjectMesh;
        std::shared_ptr<Shader> _ObjectShader;

        // Camera
        Camera _Camera;

        // Helpers
        void _LoadAssets();
        void _AppLoop();
    };
}