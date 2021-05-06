#pragma once
#include "BackendRenderPass.h"
#include "VulkanBackendStructures.h"

namespace backend::vulkan
{
    class VulkanRenderPass : public BackendRenderPass
    {
    public:
        VulkanRenderPass(const BackendRenderPassParams& params, const VulkanDevice& device, VkFormat swapchainFormat);
        ~VulkanRenderPass() override;
        void begin(const BeginRenderPassParams& params) override;
        void end() override;
        VkRenderPass getHandle() const;

    private:
        VulkanDevice device;
        VkRenderPass renderPass;
        VkFormat swapchainFormat;
    };
}
#pragma once
