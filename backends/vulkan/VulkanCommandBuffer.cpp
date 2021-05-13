#include "VulkanCommandBuffer.h"
#include "Logs.h"
#include "VulkanBackbuffer.h"
#include "VulkanBackend.h"
#include "VulkanError.h"

using namespace backend::vulkan;
using namespace coral;

DEFINE_SINGLETON(VulkanCommandBufferManager)

void VulkanCommandBufferManager::release()
{
}

VulkanCommandBufferManager::VulkanCommandBufferManager(const VulkanDevice& device, size_t count)
    : device(device)
    , count(count)
{
    // Get indices of queue families from device
    QueueFamilyIndices queueFamilyIndices = VulkanBackend::getCurrent()->getQueueFamilies();

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily; // Queue Family type that buffers from this command pool will use

    // Create a Graphics Queue Family Command Pool
    if (!VERIFY(vkCreateCommandPool(device.logicalDevice, &poolInfo, nullptr, &graphicsCommandPool)))
    {
        Logs(error) << "Failed to create a Command Pool!";
    }

    // Resize command buffer count to have one for each framebuffer
    commandBuffers.resize(count);

    VkCommandBufferAllocateInfo cbAllocInfo = {};
    cbAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cbAllocInfo.commandPool = graphicsCommandPool;
    cbAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // VK_COMMAND_BUFFER_LEVEL_PRIMARY	: Buffer you submit directly to queue. Cant be called by other buffers.
        // VK_COMMAND_BUFFER_LEVEL_SECONARY	: Buffer can't be called directly. Can be called from other buffers via "vkCmdExecuteCommands" when recording commands in primary buffer
    cbAllocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    // Allocate command buffers and place handles in array of buffers
    if (!VERIFY(vkAllocateCommandBuffers(device.logicalDevice, &cbAllocInfo, commandBuffers.data())))
    {
        Logs(error) << "Failed to allocate Command Buffers!";
    }
}

VulkanCommandBufferManager::~VulkanCommandBufferManager()
{
    vkDestroyCommandPool(device.logicalDevice, graphicsCommandPool, nullptr);
}

VkCommandBuffer& VulkanCommandBufferManager::getCommandBuffer()
{
    return commandBuffers[CURRENT_IMAGE_INDEX];
}

VkCommandPool& VulkanCommandBufferManager::getCommandPool()
{
    return graphicsCommandPool;
}

void VulkanCommandBufferManager::begin()
{
    // Information about how to begin each command buffer
    VkCommandBufferBeginInfo bufferBeginInfo = {};
    bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT; // Buffer can be resubmitted when it has already been submitted and is awaiting execution

    if (!VERIFY(vkBeginCommandBuffer(getCommandBuffer(), &bufferBeginInfo)))
    {
        Logs(error) << "Failed to start recording a Command Buffer!";
    }
}

void VulkanCommandBufferManager::end()
{
    if (!VERIFY(vkEndCommandBuffer(getCommandBuffer())))
    {
        Logs(error) << "Failed to end recording a Command Buffer!";
    }
}