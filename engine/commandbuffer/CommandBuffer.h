#pragma once

namespace coral
{
    class CommandBufferManager
    {
    public:
        CommandBufferManager(const VulkanDevice& device, size_t count);
        ~CommandBufferManager();

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
