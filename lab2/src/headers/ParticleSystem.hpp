#pragma once

// Local
#include "Assets.hpp"
#include "PSProperties.hpp"
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

    /// Class which represents a particle system, completely ready to manipulate and render.
    class ParticleSystem
    {
    public:
        /// @brief Allocates all necessary buffer objects on the graphics device.
        /// @param max_particles Takes in the maximum amount of particles
        /// inside the particle system.
        ParticleSystem(unsigned int max_particles);

        /// @brief Deallocates all resources on the graphics device.
        ~ParticleSystem();

        /// @brief Calls the updating .compute shaders
        /// @param dt The time passed since last frame/last call of the update function.
        void Update(float dt);

        /// @brief Renders the particle system onto the screen.
        /// @param cam Pointer to the application's Camera object.
        /// @param win Pointer to the application's Window object.
        void Render(std::shared_ptr<Camera> cam, std::shared_ptr<Window> win);

        /// @brief The limit of particles count in this system.
        /// @return Unsigned integer representing the maximum particles that can exist inside this system.
        inline unsigned int GetMaxParticles() const { return n_max_particles_; }

        /// @brief Loads an image texture using stb_image library as a texture to use for the particles.
        /// @param path The path to the image file.
        /// @return If the operation was sucessful or not.
        bool LoadTexture(const std::string &path);

        /// @brief Structure which holds all the editable properties of the particle system.
        PSProperties Properties;

    private:
        /// @brief Holds the max particles number and does not change through the lifetime of the object.
        unsigned int n_max_particles_;
        /// @brief Holds the number of compute groups to dispatch.
        unsigned int n_cmpt_groups_;

        /// @brief Handle for the particle SSBO.
        GLuint m_ssbo_particles_ = 0;
        /// @brief Handle for the deadlist SSBO.
        GLuint m_ssbo_deadlist_ = 0;
        /// @brief Handle for the rendering VAO.
        GLuint m_vao_ = 0;
        /// @brief Handle for the rendering VBO.
        GLuint m_vbo_ = 0;
        /// @brief Handle for the texture of an image to render on each particle quad.
        GLuint m_image = 0;

        /// @brief Helper variable to be able to spawn particles per frequency.
        float spawn_frequency_accumulator_ = 0.0f;

        /// @brief Initializes the SSBOs.
        void InitializeBuffers_();

        /// @brief Initializes the rendering buffers.
        void InitializeRendering_();

        /// @brief Binds particle SSBO in order to achieve instanced rendering.
        void BindForRendering_();
    };
}