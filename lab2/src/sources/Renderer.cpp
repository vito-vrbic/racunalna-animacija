#include "Renderer.hpp"

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer()
{
    _InitOpenGL();
}

void Renderer::Render(std::shared_ptr<RA::Camera> cam)
{
    _Clear();
}

void Renderer::_InitOpenGL()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    std::cout << "[DEBUG]: Renderer initialized with depth test + back-face culling.\n";
}

void Renderer::_Clear(float r, float g, float b, float a) const
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
