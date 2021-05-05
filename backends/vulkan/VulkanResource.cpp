#include "VulkanResource.h"

using namespace backend::vulkan;

VulkanResource::VulkanResource(const BackendResourceParams& params, const VulkanDevice& device)
    : BackendResource(params)
    , device(device)
    , ownImage(true)
{
    assert(false); // not implemented
}

VulkanResource::VulkanResource(const VulkanImage& image, const VulkanDevice& device)
    : BackendResource(BackendResourceParams())
    , image(image)
    , device(device)
    , ownImage(false)
{
}

VulkanResource::~VulkanResource()
{
    vkDestroyImageView(device.logicalDevice, image.imageView, nullptr);
    if (ownImage)
    {
        vkDestroyImage(device.logicalDevice, image.image, nullptr);
    }
}

void VulkanResource::bind(int index)
{
}

const VulkanImage& VulkanResource::getImage() const
{
    return image;
}