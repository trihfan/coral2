#include "VulkanRenderPass.h"
#include "Logs.h"
#include "VulkanBackend.h"
#include "VulkanError.h"

using namespace backend::vulkan;
using namespace coral;

VulkanRenderPass::VulkanRenderPass(const BackendRenderPassParams& params, const VulkanDevice& device, VkFormat swapchainFormat)
    : BackendRenderPass(params)
    , device(device)
{
    // Colour attachment of render pass
    VkAttachmentDescription colourAttachment = {};
    colourAttachment.format = swapchainFormat; // Format to use for attachment
    colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // Number of samples to write for multisampling
    colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Describes what to do with attachment before rendering
    colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Describes what to do with attachment after rendering
    colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // Describes what to do with stencil before rendering
    colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // Describes what to do with stencil after rendering

    // Framebuffer data will be stored as an image, but images can be given different data layouts
    // to give optimal use for certain operations
    colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Image data layout before render pass starts
    colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // Image data layout after render pass (to change to)

    // Attachment reference uses an attachment index that refers to index in the attachment list passed to renderPassCreateInfo
    VkAttachmentReference colourAttachmentReference = {};
    colourAttachmentReference.attachment = 0;
    colourAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Information about a particular subpass the Render Pass is using
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // Pipeline type subpass is to be bound to
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colourAttachmentReference;

    // Need to determine when layout transitions occur using subpass dependencies
    std::array<VkSubpassDependency, 2> subpassDependencies;

    // Conversion from VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    // Transition must happen after...
    subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL; // Subpass index (VK_SUBPASS_EXTERNAL = Special value meaning outside of renderpass)
    subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; // Pipeline stage
    subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT; // Stage access mask (memory access)
    // But must happen before...
    subpassDependencies[0].dstSubpass = 0;
    subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependencies[0].dependencyFlags = 0;

    // Conversion from VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    // Transition must happen after...
    subpassDependencies[1].srcSubpass = 0;
    subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    ;
    // But must happen before...
    subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    subpassDependencies[1].dependencyFlags = 0;

    // Create info for Render Pass
    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &colourAttachment;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
    renderPassCreateInfo.pDependencies = subpassDependencies.data();

    if (!VERIFY(vkCreateRenderPass(device.logicalDevice, &renderPassCreateInfo, nullptr, &renderPass)))
    {
        Logs(error) << "Failed to create the render pass";
    }
}

VulkanRenderPass::~VulkanRenderPass()
{
    vkDestroyRenderPass(device.logicalDevice, renderPass, nullptr);
}

VkRenderPass VulkanRenderPass::getHandle() const
{
    return renderPass;
}