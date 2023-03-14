#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace coral
{
    class CommandBufferManager
    {
    public:
        CommandBufferManager(const VkDevice& device, size_t count);
        ~CommandBufferManager();

        VkCommandBuffer& getCommandBuffer();
        VkCommandPool& getCommandPool();

        void begin();
        void end();

    private:
        VkDevice device;
        size_t count;
        VkCommandPool graphicsCommandPool;
        std::vector<VkCommandBuffer> commandBuffers;
    };
}
