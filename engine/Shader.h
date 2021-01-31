#ifndef SHADER_H
#define SHADER_H

#ifdef __APPLE__
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#else
    #include <memory_resource>
#endif

#include <filesystem>
#include <string>
#include <array>
#include <unordered_map>
#include <variant>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Object.h"
#include "utils/Singleton.h"

namespace coral
{
    class Shader : public Object
    {
    public:
        // supported shader types
        enum ShaderType : int { vertex, fragment, geometry };

        // add a shader data
        void addShaderData(ShaderType type, const std::string& data);

        // initialization
        void init() override;
        void release() override;

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
    };

    // The shader manager
    class ShaderManager
    {
        MAKE_ENGINE_SINGLETON(ShaderManager)
    public:
        // add a path to looks for loading shaders
        static void addShaderPath(const std::filesystem::path& path);

        // get the shader of the given name
        static std::shared_ptr<Shader> getShader(const std::string& name);

    private:
        ShaderManager(std::pmr::memory_resource* memory_resource);
        void iterateFolder(const std::filesystem::path& path);
        int getShaderType(const std::filesystem::path& extension) const;

    private:
        static std::vector<std::filesystem::path> paths;
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    };
}
#endif
