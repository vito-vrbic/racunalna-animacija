#pragma once

// External headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer
{
public:
    Renderer();

    void Clear(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f) const;
    void SetWireframe(bool enabled) const;

private:
    void InitOpenGL();
};