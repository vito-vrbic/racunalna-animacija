#pragma once

// Local Headers
#include "Shader.hpp"
// Standard Headers
#include <memory>
// External Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace RA
{
    class Renderable
    {
    public:
        virtual void Render(std::shared_ptr<Shader> shader, glm::mat4 view_mat, glm::mat4 pers_mat) = 0;

        Renderable();

        ~Renderable();

    protected:
        GLuint VAO;
    };
}