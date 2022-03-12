#include "VulkanFramebuffer.h"
#include "BackendResource.h"
#include "Logs.h"
#include "VulkanBackbuffer.h"
#include "VulkanError.h"
#include "VulkanRenderPass.h"
#include "VulkanResource.h"

using namespace coral;
using namespace coral;

VulkanFramebuffer::VulkanFramebuffer(const BackendFramebufferCreationParams& params, const VulkanDevice& device, const VkExtent2D& extend)
    : BackendFramebuffer(params)
    , device(device)
    , linkWithBackbuffer(params.resources.size() == 1 && params.resources[0].name == BackendFramebuffer::backbufferName)
{
    if (linkWithBackbuffer)
    {
        VulkanBackbuffer::init(params, device, extend);
    }
    else
    {
        init(params, extend);
    }
}

VulkanFramebuffer::~VulkanFramebuffer()
{
    if (!linkWithBackbuffer)
    {
        vkDestroyFramebuffer(device.logicalDevice, framebuffer, nullptr);
    }
}

VkFramebuffer VulkanFramebuffer::getHandle() const
{
    if (linkWithBackbuffer)
    {
        return VulkanBackbuffer::getCurrent()->getHandle();
    }
    return framebuffer;
}

void VulkanFramebuffer::init(const BackendFramebufferCreationParams& params, const VkExtent2D& extend)
{
    std::vector<VkImageView> attachments(params.resources.size());
    for (size_t i = 0; i < attachments.size(); i++)
    {
        attachments[i] = static_cast<const VulkanResource*>(params.resources[i].resource)->getImage().imageView;
    }

    VkFramebufferCreateInfo framebufferCreateInfo = {};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = static_cast<VulkanRenderPass*>(params.renderPass)->getHandle(); // Render Pass layout the Framebuffer will be used with
    framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferCreateInfo.pAttachments = attachments.data(); // List of attachments (1:1 with Render Pass)
    framebufferCreateInfo.width = extend.width; // Framebuffer width
    framebufferCreateInfo.height = extend.height; // Framebuffer height
    framebufferCreateInfo.layers = 1; // Framebuffer layers

    if (!VERIFY(vkCreateFramebuffer(device.logicalDevice, &framebufferCreateInfo, nullptr, &framebuffer)))
    {
        Logs(error) << "Failed to create a Framebuffer!";
    }
}