#pragma once

#include "BackendCommandBuffer.h"
#include "VulkanBackendStructures.h"

namespace backend::vulkan
{
    class VulkanCommandBuffer : public BackendCommandBuffer
    {
    public:
        VulkanCommandBuffer(const VkCommandBuffer& commandBuffer);

        void begin() override;
        void end() override;

        //
        void setViewport(float x, float y, float width, float height) override;
        void clearColor(float red, float green, float blue, float alpha) override;
        void clearDepth() override;

        // Draw
        void draw(int indexCount) override;

    private:
        VkCommandBuffer commandBuffer;
    };

    class VulkanCommandBufferManager : public BackendCommandBufferManager
    {
    public:
        VulkanCommandBufferManager(const VulkanDevice& device, size_t count);
        ~VulkanCommandBufferManager();

        BackendCommandBuffer* internalGetCommandBuffer() override;
        void internalSubmit(BackendCommandBufferStage stage) override;

    private:
        VulkanDevice device;
        size_t count;
        size_t current;
        VkCommandPool graphicsCommandPool;
        std::vector<VulkanCommandBuffer> commandBuffers;
    };
}
