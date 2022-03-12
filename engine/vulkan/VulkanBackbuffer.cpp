#include "VulkanBackbuffer.h"
#include "VulkanBackend.h"

using namespace coral;

std::atomic<bool> VulkanBackbuffer::isInit = false;
std::vector<std::unique_ptr<VulkanResource>> VulkanBackbuffer::images;
std::vector<std::unique_ptr<VulkanFramebuffer>> VulkanBackbuffer::framebuffers;

void VulkanBackbuffer::setSwapChainImages(std::vector<std::unique_ptr<VulkanResource>>&& swapchainImages)
{
    images = std::move(swapchainImages);
}

void VulkanBackbuffer::init(const BackendFramebufferCreationParams& params, const VulkanDevice& device, const VkExtent2D& extend)
{
    bool expected = false;
    if (isInit.compare_exchange_weak(expected, true, std::memory_order_relaxed))
    {
        framebuffers.resize(images.size());

        for (size_t i = 0; i < framebuffers.size(); i++)
        {
            BackendFramebufferCreationParams backbufferParams;
            backbufferParams.renderPass = params.renderPass;
            BackendFramebufferResource resource;
            resource.resource = images[i].get();
            backbufferParams.resources.push_back(resource);
            framebuffers[i] = std::make_unique<VulkanFramebuffer>(backbufferParams, device, extend);
        }
    }
}

void VulkanBackbuffer::release()
{
    isInit = false;
    framebuffers.clear();
    images.clear();
}

VulkanFramebuffer* VulkanBackbuffer::getCurrent()
{
    return framebuffers[CURRENT_IMAGE_INDEX].get();
}
