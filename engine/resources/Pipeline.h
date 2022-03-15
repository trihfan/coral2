#pragma once
#include "Object.h"
#include "vulkan/VulkanBackendStructures.h"
#include <memory>
#include <glm/glm.hpp>

namespace coral
{
    enum class CullFace
    {
        front,
        back,
        none
    };

    /**
     * @brief The PipelineParams contains the creation parameters 
     * for the pipeline
     */
    struct PipelineParams
    {
        std::string name;

        // Renderpass
        std::string renderpass;

        // Rasterization
        bool depthTest = true;
        CullFace cullFace = CullFace::back;
        bool blending = false;

        // Multisampling

        // Vertex input

        // Blending

        // Shader
        std::string vertexShader;
        std::string fragmentShader;
    };

    /**
     * @brief The Pipeline class describe a pipeline configuration for rendering
     */
    class Pipeline : public Object
    {
    public:
        // Construction
        Pipeline();

        // Properties
        Property<PipelineParams> params;
        Property<bool> dirty;

        // Set uniform value
        void setUniform(const std::string& name, bool value) const;
        void setUniform(const std::string& name, int value) const;
        void setUniform(const std::string& name, float value) const;
        void setUniform(const std::string& name, const glm::vec2& value) const;
        void setUniform(const std::string& name, const glm::vec3& value) const;
        void setUniform(const std::string& name, const glm::vec4& value) const;
        void setUniform(const std::string& name, const glm::mat2& mat) const;
        void setUniform(const std::string& name, const glm::mat3& mat) const;
        void setUniform(const std::string& name, const glm::mat4& mat) const;

        // Use
        const std::string& getRenderPassName() const;
        void use();

        // Init
        virtual void init() override;
        virtual void release() override;

    private:
        // Vulkan data
        VulkanDevice device;
        VkPipeline pipeline;
        VkPipelineLayout layout;
    };
}
