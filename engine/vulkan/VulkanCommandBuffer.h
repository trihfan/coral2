#pragma once

#include "Singleton.h"
#include "VulkanBackendStructures.h"

#define CURRENT_COMMAND_BUFFER VulkanCommandBufferManager::get()->getCommandBuffer()

namespace coral
{
    class VulkanCommandBufferManager
    {
        MAKE_SINGLETON(VulkanCommandBufferManager)
    public:
        VulkanCommandBufferManager(const VulkanDevice& device, size_t count);
        ~VulkanCommandBufferManager();

        VkCommandBuffer& getCommandBuffer();
        VkCommandPool& getCommandPool();

        void begin();
        void end();

    private:
        VulkanDevice device;
        size_t count;
        VkCommandPool graphicsCommandPool;
        std::vector<VkCommandBuffer> commandBuffers;
    };
}
