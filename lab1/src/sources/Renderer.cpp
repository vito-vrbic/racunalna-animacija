#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer()
{
    InitOpenGL();
}

void Renderer::InitOpenGL()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    std::cout << "[DEBUG]: Renderer initialized with depth test + back-face culling.\n";
}

void Renderer::Clear(float r, float g, float b, float a) const
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetWireframe(bool enabled) const
{
    glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
}
