#include "VulkanPipeline.h"
#include "Logs.h"
#include "VulkanBackend.h"
#include "VulkanShader.h"

using namespace coral;
using namespace backend::vulkan;

VulkanPipeline::VulkanPipeline(const BackendPipelineParams& params, VulkanDevice device)
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
    //viewport.width = 100;
    //viewport.height = 100;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    VkRect2D scissor {};
    scissor.offset = { 0, 0 };
    //scissor.extent = swapchainExtent;

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
}

void VulkanPipeline::use()
{
}

void VulkanPipeline::resize(int width, int height)
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
