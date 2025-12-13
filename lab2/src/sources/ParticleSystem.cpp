#include "ParticleSystem.hpp"

// Local
#include "Camera.hpp"
#include "Window.hpp"
// External
#include <stb_image.h>

using namespace RA;
using namespace RA::Assets;

ParticleSystem::ParticleSystem(unsigned int max_particles)
    : n_max_particles_(max_particles), n_cmpt_groups_((max_particles + 127) / 128)
{
    InitializeBuffers_();
    InitializeRendering_();
}

ParticleSystem::~ParticleSystem()
{
    glDeleteBuffers(1, &m_ssbo_particles_);
    glDeleteBuffers(1, &m_ssbo_deadlist_);
    glDeleteBuffers(1, &m_vbo_);
    glDeleteVertexArrays(1, &m_vao_);
}

void ParticleSystem::InitializeBuffers_()
{
    // Initialize particle SSBO.
    glGenBuffers(1, &m_ssbo_particles_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_particles_);

    // Each particle has the following structure (9 floats):
    // vec3 pos (3 floats)
    // float size (1 float)
    // vec3 velocity (3 float)
    // float age (1 float)
    // float life_length (1 float)
    std::vector<float> init_particles(n_max_particles_ * 9, 0.0f);

    // Initialize all particles as dead (set age to -1).
    for (unsigned int i = 0; i < n_max_particles_; i++)
        init_particles[i * 9 + 7] = -1.0f;

    // Store particle data inside the particle SSBO.
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * init_particles.size(), init_particles.data(), GL_DYNAMIC_DRAW);

    // Initialize deadlist SSBO.
    glGenBuffers(1, &m_ssbo_deadlist_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_deadlist_);

    // dead_indices[0] = counter, dead_indices[1..N] = free slots
    std::vector<unsigned int> init_deadlist(n_max_particles_ + 1);

    init_deadlist[0] = n_max_particles_; // All particles are dead initially.

    for (unsigned int i = 0; i < n_max_particles_; ++i)
        init_deadlist[i + 1] = i; // Every particle is in the deadlist initially.

    // Store deadlist data inside the deadlist SSBO.
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * init_deadlist.size(), init_deadlist.data(), GL_DYNAMIC_DRAW);
}

void ParticleSystem::InitializeRendering_()
{
    // Initialize the VAO.
    glGenVertexArrays(1, &m_vao_);
    glBindVertexArray(m_vao_);

    // Initialize the VBO.
    glGenBuffers(1, &m_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_);

    // Create a quad data array which holds (x,y,u,v) for each corner of the quad.
    float quad_data[] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 1.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.0f, 1.0f,  // top-left
            0.5f, 0.5f, 1.0f, 1.0f    // top-right
        };

    // Store the quad data inside the VBO.
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    // Assign vertex attribute for position (vec2).
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0 /*location in shader*/,
        2 /* vec2 */,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float) /* stride */,
        (void *)0 /* offset */
    );

    // Assign vertex attribute for UV (vec2).
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1 /*location in shader*/,
        2 /* vec2 */,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float) /* stride */,
        (void *)(2 * sizeof(float)) /* offset after x,y */
    );

    // Unbind.
    glBindVertexArray(0);
}

void ParticleSystem::BindForRendering_()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo_particles_);
}

void ParticleSystem::Update(float dt)
{
    // Check if the all shaders are loaded.
    if (!LifeCompute || !BirthCompute || !DeadResetCompute)
    {
        std::cerr << "ParticleSystem: Compute shaders not loaded!\n";
        return;
    }

    // Update the frequency accumulator and get a spawn count for this update.
    spawn_frequency_accumulator_ += Properties.Frequency * dt;
    int spawn_count = (int)spawn_frequency_accumulator_;
    spawn_frequency_accumulator_ -= spawn_count;

    // Update life of all particles.
    LifeCompute->Use();
    LifeCompute->SetUniform("delta_time", dt);
    LifeCompute->SetUniform("max_particles", (int)n_max_particles_);
    LifeCompute->SetUniform("gravity", Properties.Gravity);
    LifeCompute->SetUniform("size_falloff", Properties.SizeFalloff);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo_particles_);

    glDispatchCompute(n_cmpt_groups_, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    if (spawn_count <= 0)
    {
        spawn_count = 0;
        return;
    }

    // Rebuild deadlist SSBO data from the current particle ages.
    DeadResetCompute->Use();
    DeadResetCompute->SetUniform("max_particles", (int)n_max_particles_);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo_particles_);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssbo_deadlist_);

    glDispatchCompute(n_cmpt_groups_, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Spawn new particles into dead slots.
    BirthCompute->Use();
    BirthCompute->SetUniform("n_new_particles", spawn_count);
    BirthCompute->SetUniform("random", std::rand() % 10000);
    BirthCompute->SetUniform("maximum_life_length", Properties.MaximumLifeLength);
    BirthCompute->SetUniform("minimum_life_length", Properties.MinimumLifeLength);
    BirthCompute->SetUniform("start_velocity_strength", Properties.StartVelocityStrength);
    BirthCompute->SetUniform("maximum_start_size", Properties.MaximumStartSize);
    BirthCompute->SetUniform("minimum_start_size", Properties.MinimumStartSize);
    BirthCompute->SetUniform("src_pstn", Properties.SourcePosition);
    BirthCompute->SetUniform("src_r", Properties.SourceSphereRadius);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo_particles_);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssbo_deadlist_);

    glDispatchCompute(n_cmpt_groups_, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleSystem::Render(std::shared_ptr<Camera> cam, std::shared_ptr<Window> win)
{
    // Disable the depth mask for transparency.
    glDepthMask(GL_FALSE);

    // Don't render if there is not a render shader.
    if (!Assets::Render)
        return;

    // Set uniform variables from cam and win.
    Assets::Render->Use();
    Assets::Render->SetUniform("view", cam->GetViewMatrix());
    Assets::Render->SetUniform("proj", win->GetPerspectiveMatrix());
    Assets::Render->SetUniform("cam_right", cam->GetRight());
    Assets::Render->SetUniform("cam_up", cam->GetUp());
    Assets::Render->SetUniform("cam_forward", cam->GetFront());
    Assets::Render->SetUniform("start_color", Properties.StartColor);
    Assets::Render->SetUniform("end_color", Properties.EndColor);

    // Set the image is such exists.
    if (m_image != 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_image);
        Assets::Render->SetUniform("image", 0);
        Assets::Render->SetUniform("has_image", true);
    }
    else
    {
        Assets::Render->SetUniform("has_image", false);
    }

    // Bind the particle SSBO.
    BindForRendering_();

    // Do instanced rendering of the quad for every alive particle in the particle SSBO.
    glBindVertexArray(m_vao_);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, n_max_particles_);

    // Unbind.
    glBindVertexArray(0);

    // Enable depth mask again.
    glDepthMask(GL_TRUE);
}

bool ParticleSystem::LoadTexture(const std::string &path)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "Failed to load particle texture: " << path << "\n";
        return false;
    }

    // Create a texture on the graphics device if one doesn't already exist.
    if (m_image == 0)
        glGenTextures(1, &m_image);

    // Bind to the texture.
    glBindTexture(GL_TEXTURE_2D, m_image);

    // Send data to the texture.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Generate a mipmap for the texture.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clamping and mipmap settings for the texture.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free the allocated data on the CPU.
    stbi_image_free(data);

    return true;
}