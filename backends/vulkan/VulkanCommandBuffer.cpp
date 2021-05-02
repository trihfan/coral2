#include "VulkanCommandBuffer.h"
#include "Logs.h"

using namespace backend::vulkan;
using namespace coral;

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
