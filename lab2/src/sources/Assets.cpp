#include "Assets.hpp"

namespace RA::Assets
{
    std::shared_ptr<RA::ComputeShader> PSCompute = nullptr;
    std::shared_ptr<RA::RenderShader> PSRender = nullptr;
}

void RA::Assets::Load()
{
    PSCompute = ComputeShader::LoadShader("ps_compute");
    PSRender = RenderShader::LoadShader("ps_render");
}