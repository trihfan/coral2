#include "RenderPass.h"
#include "resources/Framebuffer.h"
#include "resources/Resource.h"

using namespace coral;

void RenderPass::render(RenderQueue& queue)
{
    //framebuffer->bind();
    internalRender(queue);
}

void RenderPass::addInput(const std::string& input)
{
    inputs.push_back(input);
}

const std::vector<std::string>& RenderPass::getInputs() const
{
    return inputs;
}

void RenderPass::addOutput(const RenderPassResource& output)
{
    outputs.push_back(output);
}

const std::vector<RenderPassResource>& RenderPass::getOutputs() const
{
    return outputs;
}

void RenderPass::prepare()
{
    clear();

    // output
    std::vector<std::pair<std::string, ResourceRole>> outputNames;
    std::vector<ResourceRole> outputRoles;

    for (const auto& output : outputs)
    {
        auto resource = ResourceManager::getResourceByName(output.name);
        outputNames.push_back(std::make_pair(output.name, output.role));

        resource->internalFormat = output.internalFormat;
        resource->format = output.format;
        resource->type = output.type;
        resource->sampleCount = output.sampleCount;
    }
    
    framebuffer = FramebufferManager::getFramebufferFor(outputNames);
}

void RenderPass::clear()
{
    framebuffer = nullptr;
}