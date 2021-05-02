#pragma once
#include "BackendRenderPass.h"
#include "VulkanBackendStructures.h"
#include <memory>

namespace backend::vulkan
{
    class VulkanRenderPass : public BackendRenderPass
    {
    public:
        VulkanRenderPass(const BackendRenderPassParams& params, const VulkanDevice& device, VkFormat swapchainFormat);
        ~VulkanRenderPass() override;

        VkRenderPass getHandle() const;

    private:
        VulkanDevice device;
        VkRenderPass renderPass;
        VkFormat swapchainFormat;
    };
}
#pragma once
