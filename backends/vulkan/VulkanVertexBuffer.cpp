#include "VulkanVertexBuffer.h"
#include "Logs.h"
#include "VulkanBufferUtils.h"
#include "VulkanCommandBuffer.h"
#include "VulkanError.h"

using namespace coral;
using namespace backend::vulkan;

VulkanVertexBuffer::VulkanVertexBuffer(const BackendVertexBufferData& data, const VulkanDevice& device, const VkQueue& transferQueue)
    : BackendVertexBuffer(data)
    , device(device)
    , indexCount(data.indicesCount)
{
    createVertexBuffer(data, transferQueue, VulkanCommandBufferManager::get()->getCommandPool());
    createIndexBuffer(data, transferQueue, VulkanCommandBufferManager::get()->getCommandPool());
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
    vkDestroyBuffer(device.logicalDevice, vertexBuffer, nullptr);
    vkFreeMemory(device.logicalDevice, vertexBufferMemory, nullptr);
    vkDestroyBuffer(device.logicalDevice, indexBuffer, nullptr);
    vkFreeMemory(device.logicalDevice, indexBufferMemory, nullptr);
}

void VulkanVertexBuffer::draw()
{
    auto commandBuffer = VulkanCommandBufferManager::get()->getCommandBuffer();

    VkBuffer vertexBuffers[] = { vertexBuffer }; // Buffers to bind
    VkDeviceSize offsets[] = { 0 }; // Offsets into buffers being bound
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets); // Command to bind vertex buffer before drawing with them

    // Bind mesh index buffer, with 0 offset and using the uint32 type
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    // Execute pipeline
    vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
}

void VulkanVertexBuffer::createVertexBuffer(const BackendVertexBufferData& data, const VkQueue& transferQueue, const VkCommandPool& transferCommandPool)
{
    // Get size of buffer needed for vertices
    VkDeviceSize bufferSize = data.vertexSize * data.verticesCount;

    // Temporary buffer to "stage" vertex data before transferring to GPU
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // Create Staging Buffer and Allocate Memory to it
    createBuffer(device.physicalDevice, device.logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagingBuffer, &stagingBufferMemory);

    // MAP MEMORY TO VERTEX BUFFER
    void* dataPtr; // 1. Create pointer to a point in normal memory
    vkMapMemory(device.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &dataPtr); // 2. "Map" the vertex buffer memory to that point
    memcpy(dataPtr, data.vertices, (size_t)bufferSize); // 3. Copy memory from vertices vector to the point
    vkUnmapMemory(device.logicalDevice, stagingBufferMemory); // 4. Unmap the vertex buffer memory

    // Create buffer with TRANSFER_DST_BIT to mark as recipient of transfer data (also VERTEX_BUFFER)
    // Buffer memory is to be DEVICE_LOCAL_BIT meaning memory is on the GPU and only accessible by it and not CPU (host)
    createBuffer(device.physicalDevice, device.logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &vertexBuffer, &vertexBufferMemory);

    // Copy staging buffer to vertex buffer on GPU
    copyBuffer(device.logicalDevice, transferQueue, transferCommandPool, stagingBuffer, vertexBuffer, bufferSize);

    // Clean up staging buffer parts
    vkDestroyBuffer(device.logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(device.logicalDevice, stagingBufferMemory, nullptr);
}

void VulkanVertexBuffer::createIndexBuffer(const BackendVertexBufferData& data, const VkQueue& transferQueue, const VkCommandPool& transferCommandPool)
{
    // Get size of buffer needed for indices
    VkDeviceSize bufferSize = sizeof(uint32_t) * data.indicesCount;

    // Temporary buffer to "stage" index data before transferring to GPU
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(device.physicalDevice, device.logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    // MAP MEMORY TO INDEX BUFFER
    void* dataPtr;
    vkMapMemory(device.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &dataPtr);
    memcpy(dataPtr, data.indices, (size_t)bufferSize);
    vkUnmapMemory(device.logicalDevice, stagingBufferMemory);

    // Create buffer for INDEX data on GPU access only area
    createBuffer(device.physicalDevice, device.logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &indexBuffer, &indexBufferMemory);

    // Copy from staging buffer to GPU access buffer
    copyBuffer(device.logicalDevice, transferQueue, transferCommandPool, stagingBuffer, indexBuffer, bufferSize);

    // Destroy + Release Staging Buffer resources
    vkDestroyBuffer(device.logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(device.logicalDevice, stagingBufferMemory, nullptr);
}
