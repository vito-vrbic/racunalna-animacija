#pragma once

// Local Headers
#include "Renderable.hpp"
#include "Transform.hpp"
// Standard Headers
#include <vector>
#include <string>
// External Headers
#include <glm/glm.hpp>

namespace RA
{
    class Polyline : public Transform, Renderable
    {
    public:
        Polyline(float size, glm::vec4 color);
        ~Polyline();

        void Render(std::shared_ptr<Shader> shader, glm::mat4 view_mat, glm::mat4 pers_mat) override;

        std::vector<glm::vec3> GetPoints() const;
        bool AddPoint(const glm::vec3 &point, int index = -1);
        bool RemovePoint(int index);

        bool SetPoints(const std::vector<glm::vec3> vec = std::vector<glm::vec3>());

    private:
        void _SetupPolyline();

        std::vector<glm::vec3> _points;

        // OpenGL buffer handles
        unsigned int _VBO;

        bool _setup;
        float _line_size;
        glm::vec4 _color;
    };
}
