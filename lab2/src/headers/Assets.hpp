#pragma once

// Local
#include "ComputeShader.hpp"
#include "RenderShader.hpp"
// Standard
#include <memory>

namespace RA
{
    namespace Assets
    {
        void Load();

        extern std::shared_ptr<RA::ComputeShader> BirthCompute;
        extern std::shared_ptr<RA::ComputeShader> LifeCompute;
        extern std::shared_ptr<RA::ComputeShader> DeadResetCompute;
        extern std::shared_ptr<RA::RenderShader> Render;
    };
};