#include "Framebuffer.h"
#include "Engine.h"
#include "Resource.h"
#include "renderpasses/RenderPass.h"

using namespace coral;

Framebuffer::Framebuffer()
{
    connect(extend.modified, *this, &Framebuffer::reset);
    connect(renderPass.modified, *this, &Framebuffer::reset);
    connect(resources.itemAdded, *this, &Framebuffer::reset);
    connect(resources.itemAdded, *this, &Framebuffer::reset);
}

void Framebuffer::init()
{
    //
    /*backend::BackendFramebufferCreationParams params;
    params.renderPass = renderpass->getBackendRenderPass();

    for (const auto& resource : resources)
    {
        params.resources.push_back(backend::BackendFramebufferResource { resource.resource->getName(), resource.role, resource.resource->getBackendResource() });
    }

    backendFramebuffer = backend::BackendObjectFactory<backend::BackendFramebuffer>::create(params);*/


    //

    linkWithBackbuffer = resources.size() == 1 && resources[0].resource->name == backbufferName;
    if (linkWithBackbuffer)
    {
        //VulkanBackbuffer::init(params, device, extend);
        return;
    }

    std::vector<VkImageView> attachments(resources.size());
    for (size_t i = 0; i < attachments.size(); i++)
    {
        attachments[i] = resources[i].getImage().imageView;
    }

    VkFramebufferCreateInfo framebufferCreateInfo = {};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = renderPass->getHandle(); // Render Pass layout the Framebuffer will be used with
    framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferCreateInfo.pAttachments = attachments.data(); // List of attachments (1:1 with Render Pass)
    framebufferCreateInfo.width = extend->x; // Framebuffer width
    framebufferCreateInfo.height = extend->y; // Framebuffer height
    framebufferCreateInfo.layers = 1; // Framebuffer layers

    if (VERIFY(vkCreateFramebuffer(device.logicalDevice, &framebufferCreateInfo, nullptr, &framebuffer)) == false)
    {
        Logs(error) << "Failed to create a Framebuffer!";
    }
}

void Framebuffer::release()
{
    if (!linkWithBackbuffer)
    {
        vkDestroyFramebuffer(device.logicalDevice, framebuffer, nullptr);
    }
}

/*VkFramebuffer VulkanFramebuffer::getHandle() const
{
    if (linkWithBackbuffer)
    {
        return VulkanBackbuffer::getCurrent()->getHandle();
    }
    return framebuffer;
}*/
