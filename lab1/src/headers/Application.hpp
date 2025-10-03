#pragma once

// Local headers
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "Assets.hpp"
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

        Application();

        void Run();

    private:
        std::unique_ptr<Window> _Window;
        std::unique_ptr<Renderer> _Renderer;

        // Assets
        Assets _Assets;

        // Camera
        Camera _Camera;

        void _Loop();
    };
}