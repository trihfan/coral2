#include "RenderPass.h"
#include "Engine.h"
#include "resources/Framebuffer.h"
#include "resources/Resource.h"

using namespace coral;

void RenderPass::render(RenderQueue& queue)
{
    if (framebuffer)
    {
        framebuffer->bind();
    }
    internalRender(queue);
}

void RenderPass::addInput(const RenderPassInput& input)
{
    inputs.push_back(input);
}

const std::vector<RenderPassInput>& RenderPass::getInputs() const
{
    return inputs;
}

void RenderPass::addOutput(const RenderPassOutput& output)
{
    outputs.push_back(output);
}

const std::vector<RenderPassOutput>& RenderPass::getOutputs() const
{
    return outputs;
}

void RenderPass::prepare()
{
    clear();

    // Output
    if (!outputs.empty())
    {
        // Setup the resources
        std::vector<std::pair<std::string, ResourceRole>> framebufferResources;
        for (const auto& output : outputs)
        {
            auto resource = ResourceManager::getResourceByName(output.name);
            if (!resource)
            {
                resource = ObjectManager::createWithName<Resource>(output.name);
                ResourceManager::registerResource(resource);

                // Configure resource
                resource->width = Engine::current_parameters.width;
                resource->height = Engine::current_parameters.height;
                resource->internalFormat = output.internalFormat;
                resource->format = output.format;
                resource->type = output.type;
                resource->sampleCount = output.sampleCount;
            }

            framebufferResources.push_back(std::make_pair(output.name, output.role));
            resources.push_back(resource);
        }
        
        // Get the framebuffer
        framebuffer = FramebufferManager::getFramebufferFor(framebufferResources);
    }
}

void RenderPass::clear()
{
    framebuffer = nullptr;
    resources.clear();
}