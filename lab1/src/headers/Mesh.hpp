#pragma once

// Local Headers
// Standard Headers
#include <limits>
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
// External Headers
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RA
{
    class Mesh
    {
    public:
        /// @brief Loads a mesh from a '.obj' file using a file reader.
        /// @param filepath The filepath of the .obj file
        /// @return shared_ptr to a Mesh object created from the file. Will be null if error.
        static std::shared_ptr<Mesh> LoadMesh(const std::string &filepath);

        Mesh() = default;
        ~Mesh() = default;

        /// @brief Vertices of the mesh. Should not be manually edited.
        std::vector<glm::vec3> Vertices;

        /// @brief Normals of the mesh. Should not be manually edited.
        std::vector<glm::vec3> Normals;

        /// @brief UV coordinates of the mesh. Should not be manually edited.
        std::vector<glm::vec2> UVCoords;

        /// @brief Indices of the mesh. Should not be manually edited.
        std::vector<unsigned int> Indices;

    private:
        /// @brief Computes per-vertex normals from vertex positions and indices.
        void _ComputeNormals();

        /// @brief Method which normalizes the mesh to fit it inside a 2x2x2 bounding box.
        void _Normalize();

        /// @brief Method which returns the bounding box of the mesh.
        std::pair<glm::vec3, glm::vec3> _GetBoundingBox();

        /// @brief Applies a transform to permanantly change positions of vertices.
        void _ApplyTransform(glm::mat4 matrix);
    };
};
