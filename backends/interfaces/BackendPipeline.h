#pragma once
#include <glm/glm.hpp>
#include <string>

namespace backend
{
    struct BackendPipelineParams
    {
        std::string vertexShaderFile;
        std::string fragmentShaderFile;
    };

    class BackendPipeline
    {
    public:
        virtual void use() = 0;
        virtual void resize(int width, int height) = 0;

        virtual void setUniform(const std::string& name, bool value) const = 0;
        virtual void setUniform(const std::string& name, int value) const = 0;
        virtual void setUniform(const std::string& name, float value) const = 0;
        virtual void setUniform(const std::string& name, const glm::vec2& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::vec3& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::vec4& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::mat2& mat) const = 0;
        virtual void setUniform(const std::string& name, const glm::mat3& mat) const = 0;
        virtual void setUniform(const std::string& name, const glm::mat4& mat) const = 0;
    };
}
