#include "Polyline.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace RA
{
    Polyline::Polyline(float line_size, glm::vec4 color)
        : Renderable(), _VBO(0), _line_size(line_size), _color(color)
    {
        _setup = false;
    }

    Polyline::~Polyline()
    {
        if (_VBO)
            glDeleteBuffers(1, &_VBO);
    }

    void Polyline::_SetupPolyline()
    {
        glBindVertexArray(VAO);

        if (_VBO == 0)
        {
            glGenBuffers(1, &_VBO);
        }

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, _points.size() * sizeof(glm::vec3), _points.data(), GL_STATIC_DRAW);

        // Vertex attribute for position (layout location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        _setup = true;
    }

    void Polyline::Render(std::shared_ptr<Shader> shader, glm::mat4 view_mat, glm::mat4 pers_mat)
    {
        if (!_setup)
            _SetupPolyline();

        shader->Use();
        shader->SetUniform("COLOR", _color);
        shader->SetUniform("MODEL_MAT", this->GetModelMatrix());
        shader->SetUniform("VIEW_MAT", view_mat);
        shader->SetUniform("PERS_MAT", pers_mat);

        glBindVertexArray(VAO);
        glLineWidth(_line_size);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(_points.size()));
        glLineWidth(1.0f);
        glBindVertexArray(0);
    }

    std::vector<glm::vec3> Polyline::GetPoints() const
    {
        return _points;
    }

    bool Polyline::AddPoint(const glm::vec3 &point, int index)
    {
        if (index < 0 || index > static_cast<int>(_points.size()))
        {
            _points.push_back(point);
        }
        else
        {
            _points.insert(_points.begin() + index, point);
        }

        _setup = false;
        return true;
    }

    bool Polyline::RemovePoint(int index)
    {
        if (index < 0 || index >= static_cast<int>(_points.size()))
            return false;

        _points.erase(_points.begin() + index);

        _setup = false;
        return true;
    }

    bool Polyline::SetPoints(const std::vector<glm::vec3> vec)
    {
        _points = vec;

        _setup = false;

        return true;
    }
}
