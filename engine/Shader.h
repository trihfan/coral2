#ifndef SHADER_H
#define SHADER_H

#ifdef __APPLE__
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#else
    #include <memory_resource>
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <array>
#include <unordered_map>
#include "Object.h"
#include "utils/Singleton.h"

namespace coral
{
    // supported shader types
    enum class ShaderType : int { vertex, geometry, fragment, count };

    class Shader : public Object
    {
    public:
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
        void setVec2(const std::string& name, float x, float y) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setVec4(const std::string& name, float x, float y, float z, float w);
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        void setMat3(const std::string& name, const glm::mat3& mat) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

    private:
        std::array<std::string, static_cast<size_t>(ShaderType::count)> shader_data;
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
        static std::shared_ptr<Shader> getShader(const std::string& name);

    private:
        ShaderManager(std::pmr::memory_resource* memory_resource);

    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    };
}
#endif
