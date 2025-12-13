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
	 * @brief Wrapper class for handling GLSL render shaders.
	 *
	 * This class loads, compiles, links, and manages vertex/fragment (and optional geometry) shaders.
	 * It also provides utility functions for setting common uniform variables.
	 */
	class RenderShader
	{
	public:
		/// @brief Load a shader program by name (helper for external use).
		static std::shared_ptr<RenderShader> LoadShader(const char *name);

		/// @brief OpenGL shader program ID.
		unsigned int ID;

		/// @brief Construct a shader program from vertex and fragment shader source files.
		RenderShader(const char *vertex_path, const char *fragment_path);

		/// @brief Construct a shader program with vertex, geometry, and fragment shaders.
		RenderShader(const char *vertex_path, const char *geometry_path, const char *fragment_path);

		/// @brief Destructor – deletes the OpenGL shader program.
		~RenderShader();

		/// @brief Activate (use) this shader program.
		void Use();

		/// @brief Set a boolean uniform.
		void SetUniform(const std::string &name, bool value) const;

		/// @brief Set an integer uniform.
		void SetUniform(const std::string &name, int value) const;

		/// @brief Set a float uniform.
		void SetUniform(const std::string &name, float value) const;

		/// @brief Set a 4x4 matrix uniform.
		void SetUniform(const std::string &name, const glm::mat4 &matrix) const;

		/// @brief Set a 4D vector uniform.
		void SetUniform(const std::string &name, const glm::vec4 &vec) const;

		/// @brief Set a 3D vector uniform.
		void SetUniform(const std::string &name, const glm::vec3 &vec) const;

		/// @brief Returns true if this shader includes a geometry stage.
		bool HasGeometry() const { return _geometry; };

	private:
		/// @brief Check for shader compiler or program linking errors.
		void _CheckCompilerErrors(unsigned int shader, std::string type);

		/// @brief Tracks whether this shader includes a geometry shader.
		bool _geometry;
	};
}