#pragma once

#include "VulkanBackendStructures.h"

namespace coral
{
    class VulkanRenderPass
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
