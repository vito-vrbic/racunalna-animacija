#include "Assets.hpp"

namespace RA::Assets
{
    std::shared_ptr<RA::ComputeShader> BirthCompute = nullptr;
    std::shared_ptr<RA::ComputeShader> LifeCompute = nullptr;
    std::shared_ptr<RA::ComputeShader> DeadResetCompute = nullptr;
    std::shared_ptr<RA::RenderShader> Render = nullptr;
}

void RA::Assets::Load()
{
    BirthCompute = ComputeShader::LoadShader("birth");
    LifeCompute = ComputeShader::LoadShader("life");
    DeadResetCompute = ComputeShader::LoadShader("deadreset");
    Render = RenderShader::LoadShader("render");
}