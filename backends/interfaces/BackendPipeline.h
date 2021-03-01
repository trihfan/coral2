#pragma once
#include <glm/glm.hpp>
#include <string>

namespace backend
{
    enum class CullFace
    {
        front,
        back,
        none
    };

    struct BackendPipelineParams
    {
        std::string vertexShaderFile;
        std::string fragmentShaderFile;

        bool depthTest = true;
        CullFace cullFace = CullFace::front;
    };

    class BackendPipeline
    {
    public:
        BackendPipeline(const BackendPipelineParams& params);
        virtual ~BackendPipeline() = default;
        virtual void use() = 0;
        virtual void resize(int width, int height) = 0; // todo remove

        virtual void setUniform(const std::string& name, bool value) const = 0;
        virtual void setUniform(const std::string& name, int value) const = 0;
        virtual void setUniform(const std::string& name, float value) const = 0;
        virtual void setUniform(const std::string& name, const glm::vec2& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::vec3& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::vec4& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::mat2& mat) const = 0;
        virtual void setUniform(const std::string& name, const glm::mat3& mat) const = 0;
        virtual void setUniform(const std::string& name, const glm::mat4& mat) const = 0;

    protected:
        const BackendPipelineParams params;
    };
}
