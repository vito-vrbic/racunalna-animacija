#include "ParticleSystem.hpp"
#include "ComputeShader.hpp"
#include "Camera.hpp"
#include "Window.hpp"

using namespace RA;
using namespace RA::Assets;

ParticleSystem::ParticleSystem(unsigned int maxParticles)
    : n_max_particles(maxParticles)
{
    _InitializeBuffers();
    _InitializeRendering();

    // Load compute shaders
    LifeCompute = ComputeShader::LoadShader("Life");
    BirthCompute = ComputeShader::LoadShader("Birth");
    DeadResetCompute = ComputeShader::LoadShader("DeadReset");
}

ParticleSystem::~ParticleSystem()
{
    glDeleteBuffers(1, &m_ssbo_particles);
    glDeleteBuffers(1, &m_ssbo_deadlist);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void ParticleSystem::_InitializeBuffers()
{
    // Particle SSBO
    glGenBuffers(1, &m_ssbo_particles);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_particles);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 sizeof(float) * 8 * n_max_particles,
                 nullptr, GL_DYNAMIC_DRAW);

    // Dead List SSBO
    glGenBuffers(1, &m_ssbo_deadlist);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_deadlist);

    // dead_indices[0] = counter, dead_indices[1..N] = free slots
    std::vector<unsigned int> deadInit(n_max_particles + 1);
    deadInit[0] = n_max_particles; // initially all free
    for (unsigned int i = 0; i < n_max_particles; ++i)
        deadInit[i + 1] = i;

    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 sizeof(unsigned int) * (n_max_particles + 1),
                 deadInit.data(), GL_DYNAMIC_DRAW);
}

void ParticleSystem::_InitializeRendering()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Quad VBO (unit quad centered at origin)
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    float quadVerts[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    // Vertex attribute for quad vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

    glBindVertexArray(0);
}

void ParticleSystem::_BindForRendering()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo_particles);
}

void ParticleSystem::Update(float dt, int new_particles)
{
    if (!LifeCompute || !BirthCompute || !DeadResetCompute)
    {
        std::cerr << "ParticleSystem: Shaders not loaded!\n";
        return;
    }

    // 1. Reset dead list counter
    DeadResetCompute->Use();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssbo_deadlist);
    glDispatchCompute(1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // 2. Life update
    LifeCompute->Use();
    LifeCompute->SetUniform("delta_time", dt);
    LifeCompute->SetUniform("max_particles", (int)n_max_particles);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo_particles);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssbo_deadlist);

    GLuint groups = (n_max_particles + 127) / 128;
    glDispatchCompute(groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // 3. Birth pass
    BirthCompute->Use();
    BirthCompute->SetUniform("n_new_particles", new_particles);
    BirthCompute->SetUniform("max_particles", (int)n_max_particles);
    BirthCompute->SetUniform("random", std::rand() % 10000);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo_particles);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssbo_deadlist);

    glDispatchCompute(groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleSystem::Render(std::shared_ptr<Camera> cam, std::shared_ptr<Window> win)
{
    if (!Assets::Render)
        return;

    Assets::Render->Use();
    Assets::Render->SetUniform("view", cam->GetViewMatrix());
    Assets::Render->SetUniform("proj", win->GetPerspectiveMatrix());

    _BindForRendering();
    glBindVertexArray(m_vao);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, n_max_particles);

    glBindVertexArray(0);
}