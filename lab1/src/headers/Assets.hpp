#pragma once

// Local headers
#include "BSpline.hpp"
#include "Mesh.hpp"
#include "Polyline.hpp"
#include "Shader.hpp"

// Standard headers
#include <memory>
#include <string>
#include <vector>

namespace RA
{
    class Assets
    {
    public:
        /// @brief Default mesh file
        static inline std::string MeshFile = "default_mesh.obj";

        /// @brief Default curve file
        static inline std::string CurveFile = "default_curve.curve";

        /// @brief Load a .crv file into a vector of points
        /// @param filename Path to the .crv file
        /// @return Vector of glm::vec3 points
        static std::vector<glm::vec3> LoadCRV(std::string filename);

        std::shared_ptr<Shader> ObjectShader;   ///< Shader used for the main object
        std::shared_ptr<Shader> PolylineShader; ///< Shader used for polylines

        std::shared_ptr<Mesh> ObjectMesh;        ///< Main object mesh
        std::shared_ptr<BSpline> BSplineCurve;   ///< B-spline curve representation
        std::shared_ptr<Polyline> ObjectTangent; ///< Polyline showing object tangents

        /// @brief Load all assets (meshes, shaders, curves)
        void LoadAssets();
    };
}