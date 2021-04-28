#pragma once
#include "BackendPipeline.h"
#include "VulkanBackendStructures.h"

namespace backend::vulkan
{
    class VulkanPipeline : public BackendPipeline
    {
    public:
        VulkanPipeline(const BackendPipelineParams& params, VulkanDevice device);
        void use() override;
        void resize(int width, int height) override;

        void setUniform(const std::string& name, bool value) const override;
        void setUniform(const std::string& name, int value) const override;
        void setUniform(const std::string& name, float value) const override;
        void setUniform(const std::string& name, const glm::vec2& value) const override;
        void setUniform(const std::string& name, const glm::vec3& value) const override;
        void setUniform(const std::string& name, const glm::vec4& value) const override;
        void setUniform(const std::string& name, const glm::mat2& mat) const override;
        void setUniform(const std::string& name, const glm::mat3& mat) const override;
        void setUniform(const std::string& name, const glm::mat4& mat) const override;

    private:
        VulkanDevice device;
    };
}
#pragma once
