// Local Headers
#include "Mesh.hpp"
// Standard Headers
#include <fstream>
#include <sstream>
// External Headers

namespace RA
{
    std::shared_ptr<Mesh> Mesh::LoadMesh(const std::string &filepath)
    {
        auto mesh = std::make_shared<Mesh>();

        std::ifstream file(filepath);

        if (!file.is_open())
        {
            std::cerr << "[ERROR]: Failed to open the provided '.OBJ' file: " << filepath << std::endl;
            exit(1);
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") // vertex position
            {
                glm::vec3 v;
                iss >> v.x >> v.y >> v.z;
                mesh->Vertices.push_back(v);
            }
            else if (prefix == "vn") // normal
            {
                glm::vec3 n;
                iss >> n.x >> n.y >> n.z;
                mesh->Normals.push_back(n);
            }
            else if (prefix == "vt") // texture coordinate
            {
                glm::vec2 uv;
                iss >> uv.x >> uv.y;
                mesh->UVCoords.push_back(uv);
            }
            else if (prefix == "f") // face indices (supports polygons)
            {
                std::vector<unsigned int> faceIndices;

                std::string vert;
                while (iss >> vert) // read until line ends
                {
                    std::replace(vert.begin(), vert.end(), '/', ' ');

                    std::istringstream viss(vert);
                    unsigned int posIdx = 0;
                    viss >> posIdx; // only position index for now

                    if (posIdx > 0) // OBJ indices start at 1
                        faceIndices.push_back(posIdx - 1);
                }

                // Triangulate polygon using fan method
                for (size_t i = 1; i + 1 < faceIndices.size(); i++)
                {
                    mesh->Indices.push_back(faceIndices[0]);
                    mesh->Indices.push_back(faceIndices[i]);
                    mesh->Indices.push_back(faceIndices[i + 1]);
                }
            }
        }

        file.close();

        if (mesh->Normals.empty())
        {
            std::cout << "[WARNING]: Normals not provided, will be computed automatically.\n";
            mesh->_ComputeNormals();
        }

        mesh->_Normalize();

        return mesh;
    }

    void Mesh::_ComputeNormals()
    {
        // Initialize normals with zeros
        Normals.assign(Vertices.size(), glm::vec3(0.0f));

        // Loop through all triangles
        for (size_t i = 0; i < Indices.size(); i += 3)
        {
            unsigned int i0 = Indices[i];
            unsigned int i1 = Indices[i + 1];
            unsigned int i2 = Indices[i + 2];

            const glm::vec3 &v0 = Vertices[i0];
            const glm::vec3 &v1 = Vertices[i1];
            const glm::vec3 &v2 = Vertices[i2];

            // Calculate face normal
            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            // Add face normal to each vertex normal
            Normals[i0] += normal;
            Normals[i1] += normal;
            Normals[i2] += normal;
        }

        // Normalize all vertex normals
        for (auto &n : Normals)
        {
            n = glm::normalize(n);
        }
    }

    void Mesh::_Normalize()
    {
        // Compute the axis-aligned bounding box.
        std::pair<glm::vec3, glm::vec3> bbox = _GetBoundingBox();

        glm::vec3 min = bbox.first;
        glm::vec3 max = bbox.second;

        // Compute the center of the bounding box.
        glm::vec3 center = (min + max) * 0.5f;

        // Compute the extent along each axis.
        glm::vec3 extent = max - min;

        // M is the maximum extent among the three axes.
        float M = std::max(std::max(extent.x, extent.y), extent.z);

        /*
            Build the transformation matrix :

            First, translate the mesh so that the center becomes the origin.

            Then, scale the mesh by 2/M so that the largest dimension is exactly 2.
        */

        glm::mat4 transform =
            glm::scale(glm::mat4(1.0f), glm::vec3(2.0f / M)) *
            glm::translate(glm::mat4(1.0f), -center);

        // Apply the transformation to all vertices.
        _ApplyTransform(transform);
    }

    std::pair<glm::vec3, glm::vec3> Mesh::_GetBoundingBox()
    {
        glm::vec3 min(std::numeric_limits<float>::max());
        glm::vec3 max(-std::numeric_limits<float>::max());

        for (const auto &vertex : Vertices)
        {
            min = glm::min(min, vertex);
            max = glm::max(max, vertex);
        }

        return {min, max};
    }

    void Mesh::_ApplyTransform(glm::mat4 matrix)
    {
        for (size_t i = 0; i < Vertices.size(); i++)
        {
            glm::vec4 v = glm::vec4(Vertices[i], 1.0f);
            v = matrix * v;
            Vertices[i] = glm::vec3(v);
        }
    }
}