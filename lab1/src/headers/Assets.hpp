#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include <memory>
#include <string>

namespace RA
{
    class Assets
    {
    public:
        static inline std::string MeshFile = "default_mesh.obj";
        static inline std::string CurveFile = "default_curve.curve";

        std::shared_ptr<Shader> ObjectShader;
        std::shared_ptr<Shader> PolylineShader;

        std::shared_ptr<Mesh> ObjectMesh;

        void LoadAssets()
        {
            ObjectShader = Shader::LoadShader("object");
            PolylineShader = Shader::LoadShader("polyline");
            ObjectMesh = Mesh::LoadMesh(MeshFile);
        }
    };
}