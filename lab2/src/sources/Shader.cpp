// Local Headers
#include "Shader.hpp"

RA::Shader::Shader(const char *vertex_path, const char *fragment_path) : _geometry(false)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Opening the files.
		vShaderFile.open(vertex_path);
		fShaderFile.open(fragment_path);
		std::stringstream vShaderStream, fShaderStream;

		// Reading file buffer contents into stream.
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Closing file handlers.
		vShaderFile.close();
		fShaderFile.close();

		// Turning streams into strings.
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "[ERROR]: Shader file has not been read successfully." << std::endl;
		exit(1);
	}

	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();

	// Compiling shaders.
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// Vertex Shader creation.
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	_CheckCompilerErrors(vertex, "VERTEX");

	// Fragment Shader creation.
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	_CheckCompilerErrors(fragment, "FRAGMENT");

	// Shader Program creation.
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	_CheckCompilerErrors(ID, "PROGRAM");

	// Shaders are deleted (they're attached to the program and are no longer necessary).
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

RA::Shader::Shader(const char *vertex_path, const char *geometry_path, const char *fragment_path) : _geometry(true)
{
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream gShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Otvaranje datoteka
		vShaderFile.open(vertex_path);
		gShaderFile.open(geometry_path);
		fShaderFile.open(fragment_path);

		std::stringstream vShaderStream, gShaderStream, fShaderStream;

		// Čitanje sadržaja u streamove
		vShaderStream << vShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Zatvaranje datoteka
		vShaderFile.close();
		gShaderFile.close();
		fShaderFile.close();

		// Pretvaranje streamova u stringove
		vertexCode = vShaderStream.str();
		geometryCode = gShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "[ERROR]: Shader files (vertex, geometry or fragment) could not be read successfully." << std::endl;
		exit(1);
	}

	const char *vShaderCode = vertexCode.c_str();
	const char *gShaderCode = geometryCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();

	unsigned int vertex, geometry, fragment;

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	_CheckCompilerErrors(vertex, "VERTEX");

	// Geometry shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	_CheckCompilerErrors(geometry, "GEOMETRY");

	// Fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	_CheckCompilerErrors(fragment, "FRAGMENT");

	// Kreiranje programa i povezivanje shader-a
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, geometry);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	_CheckCompilerErrors(ID, "PROGRAM");

	// Brisanje pojedinačnih shader objekata nakon povezivanja
	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);
}

RA::Shader::~Shader()
{
	// Deleting the program.
	glDeleteProgram(ID);
}

void RA::Shader::Use()
{
	// Using the program.
	glUseProgram(ID);
}

void RA::Shader::SetUniform(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void RA::Shader::SetUniform(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void RA::Shader::SetUniform(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void RA::Shader::SetUniform(const std::string &name, const glm::mat4 &matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void RA::Shader::SetUniform(const std::string &name, const glm::vec4 &vec) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

std::shared_ptr<RA::Shader> RA::Shader::LoadShader(const char *name)
{
	std::string path_vert = "./shaders/" + std::string(name) + ".vert";
	std::string path_frag = "./shaders/" + std::string(name) + ".frag";
	std::string path_geom = "./shaders/" + std::string(name) + ".geom";

	// Check if geometry shader exists
	std::FILE *file = std::fopen(path_geom.c_str(), "r");
	if (file)
	{
		std::fclose(file);
		return std::make_shared<Shader>(path_vert.c_str(), path_geom.c_str(), path_frag.c_str());
	}
	else
	{
		return std::make_shared<Shader>(path_vert.c_str(), path_frag.c_str());
	}
}

void RA::Shader::_CheckCompilerErrors(unsigned int shader, std::string type)
{
	int success;
	char infolog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infolog);
			std::cout << "[ERROR]: Shader compilation error of type: " + type + "\n" + infolog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infolog);
			std::cout << "[ERROR]: Shader linking error of type: " + type + "\n" + infolog << std::endl;
		}
	}
}