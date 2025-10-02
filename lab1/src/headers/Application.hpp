#pragma once

// Local Headers
#include "Mesh.hpp"
// Standard Headers
#include <iostream>
#include <memory>
// External Headers

namespace RA
{
    class Application
    {
    public:
        std::shared_ptr<Mesh> Mesh;
        void Run();

    private:
        void _Initialize();
        void _OpenWindow();
        void _GetShaders();
        void _StartAppLoop();
    };
};