#pragma once

#include "Object.h"
#include "backend/opengl/OpenGLBackend.h"
#include "utils/Singleton.h"
#include <array>
#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <variant>

namespace coral
{
    class Shader : public Object
    {
    public:
        // supported shader types
        enum ShaderType : int
        {
            vertex,
            fragment,
            geometry
        };

        // Construction
        Shader();

        // add a shader data
        void addShaderData(ShaderType type, const std::string& data);

        // use the shader
        // ------------------------------------------------------------------------
        void use();
        GLuint getId() const;

        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        void setMat3(const std::string& name, const glm::mat3& mat) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

    private:
        std::array<std::string, 3> shader_data;
        GLuint id = 0;

        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void checkCompileErrors(GLuint shader, std::string type);

        // initialization
        void init();
        void release();
    };

    // The shader manager
    class ShaderManager
    {
        MAKE_SINGLETON(ShaderManager)
    public:
        // add a path to looks for loading shaders
        static void addShaderPath(const std::filesystem::path& path);

        // get the shader of the given name
        static Handle<Shader> getShader(const std::string& name);

    private:
        ShaderManager();
        void iterateFolder(const std::filesystem::path& path);
        int getShaderType(const std::filesystem::path& extension) const;

    private:
        static std::vector<std::filesystem::path> paths;
        std::unordered_map<std::string, Handle<Shader>> shaders;
    };
}
