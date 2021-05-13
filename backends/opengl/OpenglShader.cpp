#include "OpenglShader.h"
#include "Logs.h"
#include "OpenglError.h"
#include "gl.h"
#include <sstream>

using namespace backend::opengl;
using namespace coral;

OpenglShader::OpenglShader(const std::string& name)
    : name(name)
{
}

void OpenglShader::addShaderCode(ShaderType type, const std::string& code)
{
    std::stringstream head;
#ifdef OPENGL_CORE
    head << "#version " << GLVersion.major << GLVersion.minor << "0 core" << std::endl;
#else
    head << "#version 300 es" << std::endl;
    head << "precision highp float;" << std::endl;
#endif

    head << code;
    shaderCode[static_cast<size_t>(type)] = head.str();
}

void OpenglShader::addUniformBlock(ShaderType type, const BackendUniformBlock& uniformBlock)
{
    uniformBlocks[static_cast<size_t>(type)].push_back(uniformBlock);
}

void OpenglShader::init()
{
    // 1. check shader data
    bool has_geometry = !shaderCode[geometry].empty();

    const char* vShaderCode = shaderCode[vertex].c_str();
    const char* fShaderCode = shaderCode[fragment].c_str();
    const char* gShaderCode = shaderCode[geometry].c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    if (!checkCompileErrors(vertex, "VERTEX"))
    {
        Logs(error) << vShaderCode;
    }

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    if (!checkCompileErrors(fragment, "FRAGMENT"))
    {
        Logs(error) << fShaderCode;
    }

    // if geometry shader is given, compile geometry shader
    unsigned int geometry = 0;
    if (has_geometry)
    {
#ifdef GL_GEOMETRY_SHADER
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, nullptr);
        glCompileShader(geometry);
        if (!checkCompileErrors(geometry, "GEOMETRY"))
        {
            Logs(error) << gShaderCode;
        }
#endif
    }

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    if (has_geometry)
    {
        glAttachShader(id, geometry);
    }
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (has_geometry)
    {
        glDeleteShader(geometry);
    }

    // Load unifrom blocks
    for (size_t i = 0; i < uniformBlocks.size(); i++)
    {
        uniformBlockIds[i].resize(uniformBlocks[i].size());
        for (size_t j = 0; j < uniformBlocks[i].size(); j++)
        {
            glGenBuffers(1, &uniformBlockIds[i][j]);
            glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIds[i][j]);
            glBufferData(GL_UNIFORM_BUFFER, uniformBlocks[i][j].size, NULL, GL_STATIC_DRAW);

            GLuint index = glGetUniformBlockIndex(id, uniformBlocks[i][j].name.c_str());
            glUniformBlockBinding(id, index, uniformBlocks[i][j].location);

            glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlocks[i][j].location, uniformBlockIds[i][j]);
        }
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    CHECK_OPENGL_ERROR

    // Clear
    shaderCode = {};
}

OpenglShader::~OpenglShader()
{
    glDeleteProgram(id);
    id = 0;
}

void OpenglShader::use()
{
    glUseProgram(id);
}

GLuint OpenglShader::getId() const
{
    return id;
}

void OpenglShader::setUniform(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value ? 1 : 0);
}

void OpenglShader::setUniform(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void OpenglShader::setUniform(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void OpenglShader::setUniform(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void OpenglShader::setUniform(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void OpenglShader::setUniform(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void OpenglShader::setUniform(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void OpenglShader::setUniform(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void OpenglShader::setUniform(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void OpenglShader::setBlockBufferSubData(const std::string& name, size_t offset, size_t size, void* data)
{
    for (size_t i = 0; i < uniformBlocks.size(); i++)
    {
        for (size_t j = 0; j < uniformBlocks[i].size(); j++)
        {
            if (uniformBlocks[i][j].name == name)
            {
                glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIds[i][j]);
                glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }
        }
    }
}

bool OpenglShader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            Logs(error) << "Error compiling " << type << " shader " << name;
            Logs(error) << infoLog;
            Logs(error) << "-----------------------------------------------------";
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            Logs(error) << "Error linking " << type << " " << name;
            Logs(error) << infoLog;
            Logs(error) << "-----------------------------------------------------";
        }
    }

    return success;
}
