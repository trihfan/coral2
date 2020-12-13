#include "Shader.h"
#include "utils/Logs.h"
#include "utils/FileUtils.h"

using namespace coral;

//*********************************************************************************
// Shader

void Shader::addShaderData(ShaderType type, const std::string& data)
{
    shader_data[static_cast<size_t>(type)] = data;
}

void Shader::init()
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
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if (has_geometry)
    {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, nullptr);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
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

void Shader::release()
{
    glDeleteProgram(id);
    id = 0;
}

void Shader::use()
{
    glUseProgram(id);
}

GLuint Shader::getId() const
{
    return id;
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value ? 1 : 0);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            Logs(error) << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            Logs(error) << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

//*********************************************************************************
// ShaderManager

DEFINE_SINGLETON(ShaderManager)
std::vector<std::filesystem::path> ShaderManager::paths;

void ShaderManager::addShaderPath(const std::filesystem::path& path)
{
    paths.push_back(path);
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& name)
{
    auto it = instance->shaders.find(name);
    if (it != instance->shaders.end())
    {
        return it->second;
    }

    Logs(error) << "unknown shader " << name;
    return nullptr;
}

ShaderManager::ShaderManager(std::pmr::memory_resource* memory_resource)
{
    // load shaders
    for (const std::filesystem::path& path : paths)
    {
        iterateFolder(path.is_relative() ? FileUtils::getAbsolutePath(path) : path);
    }
}

void ShaderManager::iterateFolder(const std::filesystem::path& path)
{
    // list shaders in folder
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(entry))
        {
            iterateFolder(entry.path());
        }
        else if (std::filesystem::is_regular_file(entry))
        {
            int shader_type = getShaderType(entry.path().extension());
            if (shader_type == -1)
            {
                Logs(warning) << "unknown shader type " << entry.path();
            }
            else
            {
                std::string name = entry.path().stem().string();
                std::shared_ptr<Shader> shader = shaders[name];
                if (!shader)
                {
                    shader = ObjectManager::create<Shader>();
                    shaders[name] = shader;
                }
                shader->addShaderData(static_cast<Shader::ShaderType>(shader_type), FileUtils::readAll(entry.path()));
            }
        }
    }
}

int ShaderManager::getShaderType(const std::filesystem::path& extension) const
{
    // vertex
    if (extension == ".vert" || extension == ".vs" || extension == ".vs.glsl")
    {
        return Shader::vertex;
    }

    // fragment
    if (extension == ".frag" || extension == ".fs" || extension == ".fs.glsl")
    {
        return Shader::fragment;
    }
   
    // geometry
    if (extension == ".geom" || extension == ".gs" || extension == ".gs.glsl")
    {
        return Shader::geometry;
    }

    return -1;
}