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

void OpenglShader::addShaderData(ShaderType type, const std::string& data)
{
    std::stringstream head;
#ifdef OPENGL_CORE
    head << "#version " << GLVersion.major << GLVersion.minor << "0 core" << std::endl;
#else
    head << "#version 300 es" << std::endl;
    head << "precision highp float;" << std::endl;
#endif

    head << data;
    shader_data[static_cast<size_t>(type)] = head.str();
}

void OpenglShader::init()
{
    // 1. check shader data
    bool has_geometry = !shader_data[geometry].empty();

    const char* vShaderCode = shader_data[vertex].c_str();
    const char* fShaderCode = shader_data[fragment].c_str();
    const char* gShaderCode = shader_data[geometry].c_str();

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

    shader_data = {};
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
    auto t = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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
