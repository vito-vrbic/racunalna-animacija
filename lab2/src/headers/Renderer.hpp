#pragma once

// Local headers
#include "Camera.hpp"
#include "Assets.hpp"

// Standard headers
#include <memory>

class Renderer
{
public:
    Renderer();
    void Render(std::shared_ptr<RA::Camera> cam);

private:
    void _InitOpenGL();
    void _Clear(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f) const;
};