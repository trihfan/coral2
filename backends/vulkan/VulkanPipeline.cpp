#include "VulkanPipeline.h"
#include "Logs.h"
#include "VulkanBackend.h"
#include "VulkanError.h"
#include "VulkanRenderPass.h"
#include "VulkanShader.h"

using namespace coral;
using namespace backend::vulkan;

VulkanPipeline::VulkanPipeline(const BackendPipelineParams& params, const VulkanDevice& device, const VkExtent2D& extend)
    : BackendPipeline(params)
    , device(device)
{
    // Create Shader Modules
    VulkanShader vertexShaderModule(params.vertexShaderFile, device.logicalDevice);
    VulkanShader fragmentShaderModule(params.fragmentShaderFile, device.logicalDevice);

    // Vertex Stage creation information
    VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
    vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; // Shader Stage name
    vertexShaderCreateInfo.module = vertexShaderModule; // Shader module to be used by stage
    vertexShaderCreateInfo.pName = "main"; // Entry point in to shader

    // Fragment Stage creation information
    VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
    fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; // Shader Stage name
    fragmentShaderCreateInfo.module = fragmentShaderModule; // Shader module to be used by stage
    fragmentShaderCreateInfo.pName = "main"; // Entry point in to shader

    // Put shader stage creation info in to array
    // Graphics Pipeline creation info requires array of shader stage creates
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

    // CREATE PIPELINE
    // Vertex input
    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo {};
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
    vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;
    vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
    vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo {};
    inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

    // Viewport and scissor
    VkViewport viewport {};
    viewport.x = 0.f;
    viewport.y = 0.f;
    viewport.width = extend.width;
    viewport.height = extend.height;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    VkRect2D scissor {};
    scissor.offset = { 0, 0 };
    scissor.extent = extend;

    VkPipelineViewportStateCreateInfo viewportCreateInfo {};
    viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportCreateInfo.viewportCount = 1;
    viewportCreateInfo.pViewports = &viewport;
    viewportCreateInfo.scissorCount = 1;
    viewportCreateInfo.pScissors = &scissor;

    // Dynamic states
    std::vector<VkDynamicState> dynamicStates {
        VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR // Dynamic viewport to allow resize in command buffer
    };

    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo {};
    dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = {};
    rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerCreateInfo.depthClampEnable = VK_FALSE; // Change if fragments beyond near/far planes are clipped (default) or clamped to plane
    rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE; // Whether to discard data and skip rasterizer. Never creates fragments, only suitable for pipeline without framebuffer output
    rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL; // How to handle filling points between vertices
    rasterizerCreateInfo.lineWidth = 1.0f; // How thick lines should be when drawn
    rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT; // Which face of a tri to cull
    rasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; // Winding to determine which side is front
    rasterizerCreateInfo.depthBiasEnable = VK_FALSE; // Whether to add depth bias to fragments (good for stopping "shadow acne" in shadow mapping)

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo = {};
    multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisamplingCreateInfo.sampleShadingEnable = VK_FALSE; // Enable multisample shading or not
    multisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // Number of samples to use per fragment

    // Blending
    // Blending decides how to blend a new colour being written to a fragment, with the old value

    // Blend Attachment State (how blending is handled)
    VkPipelineColorBlendAttachmentState colourState = {};
    colourState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT // Colours to apply blending to
        | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colourState.blendEnable = VK_TRUE; // Enable blending

    // Blending uses equation: (srcColorBlendFactor * new colour) colorBlendOp (dstColorBlendFactor * old colour)
    colourState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colourState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colourState.colorBlendOp = VK_BLEND_OP_ADD;

    // Summarised: (VK_BLEND_FACTOR_SRC_ALPHA * new colour) + (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * old colour)
    //			   (new colour alpha * new colour) + ((1 - new colour alpha) * old colour)

    colourState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colourState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colourState.alphaBlendOp = VK_BLEND_OP_ADD;
    // Summarised: (1 * new alpha) + (0 * old alpha) = new alpha

    VkPipelineColorBlendStateCreateInfo colourBlendingCreateInfo = {};
    colourBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colourBlendingCreateInfo.logicOpEnable = VK_FALSE; // Alternative to calculations is to use logical operations
    colourBlendingCreateInfo.attachmentCount = 1;
    colourBlendingCreateInfo.pAttachments = &colourState;

    // -- PIPELINE LAYOUT (TODO: Apply Future Descriptor Set Layouts) --
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 0;
    pipelineLayoutCreateInfo.pSetLayouts = nullptr;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

    // Create Pipeline Layout
    if (!VERIFY(vkCreatePipelineLayout(device.logicalDevice, &pipelineLayoutCreateInfo, nullptr, &layout)))
    {
        Logs(error) << "Failed to create Pipeline Layout!";
        return;
    }

    // -- DEPTH STENCIL TESTING --
    // TODO: Set up depth stencil testing

    // -- GRAPHICS PIPELINE CREATION --
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2; // Number of shader stages
    pipelineCreateInfo.pStages = shaderStages; // List of shader stages
    pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo; // All the fixed function pipeline states
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
    pipelineCreateInfo.pViewportState = &viewportCreateInfo;
    pipelineCreateInfo.pDynamicState = nullptr;
    pipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
    pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
    pipelineCreateInfo.pColorBlendState = &colourBlendingCreateInfo;
    pipelineCreateInfo.pDepthStencilState = nullptr;
    pipelineCreateInfo.layout = layout; // Pipeline Layout pipeline should use
    pipelineCreateInfo.renderPass = static_cast<const VulkanRenderPass*>(params.renderpass)->getHandle(); // Render pass description the pipeline is compatible with
    pipelineCreateInfo.subpass = 0; // Subpass of render pass to use with pipeline

    // Pipeline Derivatives : Can create multiple pipelines that derive from one another for optimisation
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Existing pipeline to derive from...
    pipelineCreateInfo.basePipelineIndex = -1; // or index of pipeline being created to derive from (in case creating multiple at once)

    // Create Graphics Pipeline
    if (!VERIFY(vkCreateGraphicsPipelines(device.logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline)))
    {
        Logs(error) << "Failed to create a Graphics Pipeline!";
        return;
    }

    // Destroy Shader Modules, no longer needed after Pipeline created
    vkDestroyShaderModule(device.logicalDevice, fragmentShaderModule, nullptr);
    vkDestroyShaderModule(device.logicalDevice, vertexShaderModule, nullptr);
}

VulkanPipeline::~VulkanPipeline()
{
    vkDestroyPipeline(device.logicalDevice, pipeline, nullptr);
    vkDestroyPipelineLayout(device.logicalDevice, layout, nullptr);
}

void VulkanPipeline::use()
{
}

void VulkanPipeline::setUniform(const std::string& name, bool value) const
{
}

void VulkanPipeline::setUniform(const std::string& name, int value) const
{
}

void VulkanPipeline::setUniform(const std::string& name, float value) const
{
}

void VulkanPipeline::setUniform(const std::string& name, const glm::vec2& value) const
{
}

void VulkanPipeline::setUniform(const std::string& name, const glm::vec3& value) const
{
}

void VulkanPipeline::setUniform(const std::string& name, const glm::vec4& value) const
{
}

void VulkanPipeline::setUniform(const std::string& name, const glm::mat2& mat) const
{
}

void VulkanPipeline::setUniform(const std::string& name, const glm::mat3& mat) const
{
}

void VulkanPipeline::setUniform(const std::string& name, const glm::mat4& mat) const
{
}
