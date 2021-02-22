#include "RenderPass.h"
#include "Engine.h"
#include "ObjectFactory.h"
#include "RenderPassFramebufferManager.h"
#include "RenderPassResourceManager.h"
#include "resources/Framebuffer.h"
#include "resources/Resource.h"

using namespace coral;

void RenderPass::render(RenderQueue& queue, const RenderParameters& parameters)
{
    if (framebuffer)
    {
        framebuffer->bind();
    }
    internalRender(queue, parameters);
}

void RenderPass::addInput(const RenderPassResource& input)
{
    inputs.push_back(input);
}

const std::vector<RenderPassResource>& RenderPass::getInputs() const
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

void RenderPass::prepare(const RenderParameters& parameters)
{
    invalidate();

    // Inputs
    inputResources.resize(inputs.size());
    for (size_t i = 0; i < inputs.size(); i++)
    {
        inputResources[i] = getResource(inputs[i], parameters);
    }

    // Outputs
    std::vector<FramebufferResource> framebufferResources;
    outputResources.resize(outputs.size());
    for (size_t i = 0; i < outputs.size(); i++)
    {
        outputResources[i] = getResource(outputs[i], parameters);
        framebufferResources.push_back(FramebufferResource { outputResources[i], outputs[i].role });
    }

    // Get the framebuffer
    framebuffer = RenderPassFramebufferManager::getFramebufferFor(framebufferResources);
}

void RenderPass::invalidate()
{
    framebuffer = nullptr;
    inputResources.clear();
    outputResources.clear();
}

Handle<Resource> RenderPass::getResource(const RenderPassResource& resource, const RenderParameters& parameters) const
{
    auto allocatedResource = RenderPassResourceManager::getResourceByName(resource.name);
    if (!allocatedResource)
    {
        allocatedResource = ObjectFactory::createWithName<Resource>(resource.name);
        RenderPassResourceManager::registerResource(allocatedResource);

        // Configure resource
        allocatedResource->width = parameters.width;
        allocatedResource->height = parameters.height;
        allocatedResource->internalFormat = resource.internalFormat;
        allocatedResource->format = resource.format;
        allocatedResource->type = resource.type;
        allocatedResource->sampleCount = resource.sampleCount;
    }
    return allocatedResource;
}
