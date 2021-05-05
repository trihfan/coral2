#include "VulkanCommandBuffer.h"
#include "Logs.h"
#include "VulkanBackend.h"
#include "VulkanError.h"

using namespace backend::vulkan;
using namespace coral;

VulkanCommandBuffer::VulkanCommandBuffer(const VkCommandBuffer& commandBuffer)
    : commandBuffer(commandBuffer)
{
}

void VulkanCommandBuffer::begin()
{
    //vkBeginCommandBuffer(commandBuffers[currentImage], &bufferBeginInfo);
}

void VulkanCommandBuffer::end()
{
    //vkEndCommandBuffer(commandBuffers[currentImage]);
}

void VulkanCommandBuffer::setViewport(float x, float y, float width, float height)
{
}

void VulkanCommandBuffer::clearColor(float red, float green, float blue, float alpha)
{
}

void VulkanCommandBuffer::clearDepth()
{
}

void VulkanCommandBuffer::draw(int indexCount)
{
    //vkCmdDrawIndexed(commandBuffers[currentImage], thisModel.getMesh(k)->getIndexCount(), 1, 0, 0, 0);
}

/******************************************************/

VulkanCommandBufferManager::VulkanCommandBufferManager(const VulkanDevice& device, size_t count)
    : device(device)
    , count(count)
{
    current = 0;

    // Get indices of queue families from device
    QueueFamilyIndices queueFamilyIndices = static_cast<VulkanBackend*>(Backend::current())->getQueueFamilies();

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily; // Queue Family type that buffers from this command pool will use

    // Create a Graphics Queue Family Command Pool
    if (!VERIFY(vkCreateCommandPool(device.logicalDevice, &poolInfo, nullptr, &graphicsCommandPool)))
    {
        Logs(error) << "Failed to create a Command Pool!";
    }

    // Resize command buffer count to have one for each framebuffer
    std::vector<VkCommandBuffer> vkCommandBuffers(count);

    VkCommandBufferAllocateInfo cbAllocInfo = {};
    cbAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cbAllocInfo.commandPool = graphicsCommandPool;
    cbAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // VK_COMMAND_BUFFER_LEVEL_PRIMARY	: Buffer you submit directly to queue. Cant be called by other buffers.
        // VK_COMMAND_BUFFER_LEVEL_SECONARY	: Buffer can't be called directly. Can be called from other buffers via "vkCmdExecuteCommands" when recording commands in primary buffer
    cbAllocInfo.commandBufferCount = static_cast<uint32_t>(vkCommandBuffers.size());

    // Allocate command buffers and place handles in array of buffers
    if (!VERIFY(vkAllocateCommandBuffers(device.logicalDevice, &cbAllocInfo, vkCommandBuffers.data())))
    {
        Logs(error) << "Failed to allocate Command Buffers!";
    }

    // Create the command buffer
    for (size_t i = 0; i < vkCommandBuffers.size(); i++)
    {
        commandBuffers.push_back(VulkanCommandBuffer(vkCommandBuffers[i]));
    }
}

VulkanCommandBufferManager::~VulkanCommandBufferManager()
{
    vkDestroyCommandPool(device.logicalDevice, graphicsCommandPool, nullptr);
}

backend::BackendCommandBuffer* VulkanCommandBufferManager::internalGetCommandBuffer()
{
    return &commandBuffers[current];
}

void VulkanCommandBufferManager::internalSubmit(BackendCommandBufferStage stage)
{
    if (stage == BackendCommandBufferStage::draw)
    {
        current = (current + 1) % commandBuffers.size();
    }
}