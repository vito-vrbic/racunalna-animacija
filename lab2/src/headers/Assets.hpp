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

        extern std::shared_ptr<RA::ComputeShader> PSCompute;
        extern std::shared_ptr<RA::RenderShader> PSRender;
    };
};