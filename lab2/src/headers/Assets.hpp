#pragma once

// Local headers
#include "Shader.hpp"

// Standard headers
#include <memory>
#include <string>
#include <vector>

namespace RA
{
    class Assets
    {
    public:
        static std::shared_ptr<Shader> ParticleSystemShader; ///< Shader used for the main object

        /// Here texture images loaded with std_image will be added.

        /// @brief The assets are loaded here when possible.
        static void LoadAssets();
    };
}