#pragma once

// Standard
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
// External
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RA
{
    /**
     * @brief Wrapper class for handling GLSL compute shaders.
     *
     * This class loads, compiles, links, and manages a compute shader.
     * It also provides utility functions for setting common uniform variables.
     */
    class ComputeShader
    {
    public:
        /// @brief Load a compute shader by name (helper for external use).
        static std::shared_ptr<ComputeShader> LoadShader(const char *name);

        /// @brief OpenGL shader program ID.
        unsigned int ID;

        /// @brief Construct a compute shader from source file.
        ComputeShader(const char *compute_path);

        /// @brief Destructor – deletes the OpenGL shader program.
        ~ComputeShader();

        /// @brief Activate (use) this shader program.
        void Use();

        /// @brief Set a boolean uniform.
        void SetUniform(const std::string &name, bool value) const;

        /// @brief Set an integer uniform.
        void SetUniform(const std::string &name, int value) const;

        /// @brief Set an unsigned integer uniform.
        void SetUniform(const std::string &name, unsigned int value) const;

        /// @brief Set a float uniform.
        void SetUniform(const std::string &name, float value) const;

        /// @brief Set a 4x4 matrix uniform.
        void SetUniform(const std::string &name, const glm::mat4 &matrix) const;

        /// @brief Set a 4D vector uniform.
        void SetUniform(const std::string &name, const glm::vec4 &vec) const;

        /// @brief Set a 3D vector uniform.
        void SetUniform(const std::string &name, const glm::vec3 &vec) const;

    private:
        /// @brief Check for shader compiler or program linking errors.
        void _CheckCompilerErrors(unsigned int shader, std::string type);
    };
}