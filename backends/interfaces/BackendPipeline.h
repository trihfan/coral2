#pragma once
#include <glm/glm.hpp>
#include <string>

namespace backend
{
    class BackendRenderPass;

    enum class CullFace
    {
        front,
        back,
        none
    };

    struct BackendPipelineParams
    {
        // Renderpass
        backend::BackendRenderPass* renderpass;

        // Rasterization
        bool depthTest = true;
        CullFace cullFace = CullFace::back;

        // Multisampling

        // Vertex input

        // Blending

        // Shader
        std::string vertexShaderFile;
        std::string fragmentShaderFile;
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
