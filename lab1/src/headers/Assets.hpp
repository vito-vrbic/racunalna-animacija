#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Polyline.hpp"
#include <memory>
#include <vector>
#include <string>

namespace RA
{
    class Assets
    {
    public:
        static inline std::string MeshFile = "default_mesh.obj";
        static inline std::string CurveFile = "default_curve.curve";
        static std::vector<glm::vec3> LoadCRV(std::string filename);

        std::shared_ptr<Shader> ObjectShader;
        std::shared_ptr<Shader> PolylineShader;

        std::shared_ptr<Mesh> ObjectMesh;
        std::shared_ptr<Polyline> PolygonPolyline;

        void LoadAssets();
    };
}