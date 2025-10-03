#include "Assets.hpp"

namespace RA
{
    std::vector<glm::vec3> Assets::LoadCRV(std::string filename)
    {
        std::vector<glm::vec3> points;
        std::ifstream file(filename);

        if (!file.is_open())
        {
            std::cerr << "[ERROR] Failed to open curve file: " << filename << std::endl;
            return points;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty())
                continue;

            std::istringstream iss(line);
            float x, y, z;
            if (iss >> x >> y >> z)
            {
                points.emplace_back(x, y, z);
            }
            else
            {
                std::cerr << "[WARNING] Invalid line in curve file: " << line << std::endl;
            }
        }

        file.close();
        return points;
    }
    void Assets::LoadAssets()
    {
        ObjectShader = Shader::LoadShader("object");
        PolylineShader = Shader::LoadShader("polyline");
        ObjectMesh = Mesh::LoadMesh(MeshFile);
        BSplineCurve = std::make_shared<BSpline>();
        BSplineCurve->SetControlPoints(LoadCRV(CurveFile));
        ObjectTangent = std::make_shared<Polyline>(5.0f, glm::vec4(0.7f, 0.4f, 0.11f, 1.f));
        ObjectTangent->AddPoint(glm::vec3(0.f, 0.f, 0.f));
        ObjectTangent->AddPoint(glm::vec3(0.f, 0.f, 3.f));
    }
}