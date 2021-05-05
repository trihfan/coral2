#pragma once

#include "VulkanFramebuffer.h"
#include "VulkanResource.h"
#include <atomic>
#include <memory>

namespace backend::vulkan
{
    class VulkanBackbuffer
    {
    public:
        static void setSwapChainImages(std::vector<std::unique_ptr<VulkanResource>>&& swapchainImages);
        static void init(const BackendFramebufferCreationParams& params, const VulkanDevice& device, const VkExtent2D& extend);
        static void release();

        static VulkanFramebuffer* getCurrent();
        static void next();

    private:
        static std::atomic<bool> isInit;
        static std::vector<std::unique_ptr<VulkanResource>> images;
        static std::vector<std::unique_ptr<VulkanFramebuffer>> framebuffers;
        static size_t current;
    };
}
