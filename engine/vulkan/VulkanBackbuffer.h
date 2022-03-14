#pragma once

#include "VulkanFramebuffer.h"
#include "VulkanResource.h"
#include <atomic>
#include <memory>

namespace coral
{
    class VulkanBackbuffer
    {
    public:
        static void setSwapChainImages(std::vector<std::unique_Handle<VulkanResource>>&& swapchainImages);
        static void init(const BackendFramebufferCreationParams& params, const VulkanDevice& device, const VkExtent2D& extend);
        static void release();

        static VulkanFramebuffer* getCurrent();

    private:
        static std::atomic<bool> isInit;
        static std::vector<std::unique_Handle<VulkanResource>> images;
        static std::vector<std::unique_Handle<VulkanFramebuffer>> framebuffers;
    };
}
