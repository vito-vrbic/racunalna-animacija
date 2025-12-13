// Standard
// External
#include <glm/glm.hpp>

namespace RA
{
    struct PSProperties
    {
        /// @brief The maximum life length a particle can have.
        float MaximumLifeLength = 1;

        /// @brief The minimum life length a particle can have.
        float MinimumLifeLength = 0;

        /// @brief The starting color - particles with age == 1 will have this colored tint.
        glm::vec4 StartColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

        /// @brief The ending color - particles with age == 0 will have this colored tint.
        glm::vec4 EndColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

        /// @brief The frequency with which to spawn particles
        float Frequency = 1.f;

        /// @brief Strength of the velocity.
        float StartVelocityStrength = 1.f;

        /// @brief Maximum starting size a particle can have.
        float MaximumStartSize = 1.f;

        /// @brief Minimum starting size a particle can have.
        float MinimumStartSize = 0.f;

        /// @brief The linear size falloff factor per second.
        float SizeFalloff = 1.f;

        /// @brief The position of the source where the particles initially spawn.
        glm::vec3 SourcePosition = glm::vec3(0.0f, 0.0f, 0.0f);

        /// @brief The radius of the source sphere per axis.
        glm::vec3 SourceSphereRadius = glm::vec3(1.0f, 1.0f, 1.0f);

        /// @brief Does gravity affect the particles?
        bool Gravity = true;
    };
};