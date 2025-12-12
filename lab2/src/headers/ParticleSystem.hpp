#pragma once

// Local
#include "Assets.hpp"
// Standard
#include <iostream>
#include <memory>
#include <vector>
// External
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace RA
{
    class Camera;
    class Window;

    class ParticleSystem
    {
    public:
        ParticleSystem(unsigned int maxParticles);
        ~ParticleSystem();

        void Update(float dt, int new_particles);
        void Render(std::shared_ptr<Camera> cam, std::shared_ptr<Window> win);

        inline unsigned int GetMaxParticles() const { return n_max_particles; }

        void Debug();

    private:
        unsigned int n_max_particles;

        // SSBO handles
        GLuint m_ssbo_particles = 0;
        GLuint m_ssbo_deadlist = 0;

        // VAO/VBO for rendering
        GLuint m_vao = 0;
        GLuint m_vbo = 0;

        // Helpers
        void _InitializeBuffers();
        void _InitializeRendering();
        void _BindForRendering();
    };
}