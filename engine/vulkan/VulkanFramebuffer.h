#pragma once

#include "VulkanBackendStructures.h"

namespace coral
{
    /**
     * 
     */
    class VulkanFramebuffer
    {
    public:
        VulkanFramebuffer(const BackendFramebufferCreationParams& params, const VulkanDevice& device, const VkExtent2D& extend);
        ~VulkanFramebuffer() override;
        VkFramebuffer getHandle() const;

    private:
        VulkanDevice device;
        VkFramebuffer framebuffer;
        bool linkWithBackbuffer;

        // Default init, create the framebuffer
        void init(const BackendFramebufferCreationParams& params, const VkExtent2D& extend);
    };
}
