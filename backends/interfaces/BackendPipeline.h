#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace backend
{
    class BackendRenderPass;

    enum class CullFace
    {
        front,
        back,
        none
    };

    struct BackendUniformBlock
    {
        std::string name;
        int location;
        size_t size;
    };

    struct BackendShaderParams
    {
        // Vertex Shader
        std::string vertexShader;
        std::vector<BackendUniformBlock> vertexShaderUniformBlocks;

        // Fragment Shader
        std::string fragmentShader;
        std::vector<BackendUniformBlock> fragmentShaderUniformBlocks;
    };

    struct BackendPipelineParams
    {
        std::string name;

        // Renderpass
        backend::BackendRenderPass* renderpass;

        // Rasterization
        bool depthTest = true;
        CullFace cullFace = CullFace::back;
        bool blending = false;

        // Multisampling

        // Vertex input

        // Blending

        // Shader
        BackendShaderParams shader;
    };

    class BackendPipeline
    {
    public:
        BackendPipeline(const BackendPipelineParams& params) { }
        virtual ~BackendPipeline() = default;

        virtual void use() = 0;

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
