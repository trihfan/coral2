#pragma once
#include "BackendVertexBuffer.h"
#include "VulkanBackendStructures.h"

namespace coral
{
    class VulkanVertexBuffer : public BackendVertexBuffer
    {
    public:
        VulkanVertexBuffer(const BackendVertexBufferData& data, const VulkanDevice& device, const VkQueue& transferQueue);
        ~VulkanVertexBuffer() override;
        void draw() override;

    private:
        VulkanDevice device;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;

        int indexCount;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        void createVertexBuffer(const BackendVertexBufferData& data, const VkQueue& transferQueue, const VkCommandPool& transferCommandPool);
        void createIndexBuffer(const BackendVertexBufferData& data, const VkQueue& transferQueue, const VkCommandPool& transferCommandPool);
    };
}
