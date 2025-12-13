#include "ComputeShader.hpp"

RA::ComputeShader::ComputeShader(const char *compute_path)
{
    std::string computeCode;
    std::ifstream cShaderFile;
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Open compute shader file
        cShaderFile.open(compute_path);
        std::stringstream cShaderStream;
        cShaderStream << cShaderFile.rdbuf();
        cShaderFile.close();
        computeCode = cShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "[ERROR]: Compute shader file could not be read successfully." << std::endl;
        exit(1);
    }

    const char *cShaderCode = computeCode.c_str();

    // Compile compute shader
    unsigned int compute;
    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &cShaderCode, NULL);
    glCompileShader(compute);
    _CheckCompilerErrors(compute, "COMPUTE");

    // Create program and link
    ID = glCreateProgram();
    glAttachShader(ID, compute);
    glLinkProgram(ID);
    _CheckCompilerErrors(ID, "PROGRAM");

    // Delete shader object
    glDeleteShader(compute);
}

RA::ComputeShader::~ComputeShader()
{
    glDeleteProgram(ID);
}

void RA::ComputeShader::Use()
{
    glUseProgram(ID);
}

void RA::ComputeShader::SetUniform(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void RA::ComputeShader::SetUniform(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void RA::ComputeShader::SetUniform(const std::string &name, unsigned int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void RA::ComputeShader::SetUniform(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void RA::ComputeShader::SetUniform(const std::string &name, const glm::mat4 &matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void RA::ComputeShader::SetUniform(const std::string &name, const glm::vec4 &vec) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void RA::ComputeShader::SetUniform(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

std::shared_ptr<RA::ComputeShader> RA::ComputeShader::LoadShader(const char *name)
{
    std::string path = "./shaders/" + std::string(name) + ".compute";
    return std::make_shared<ComputeShader>(path.c_str());
}

void RA::ComputeShader::_CheckCompilerErrors(unsigned int shader, std::string type)
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