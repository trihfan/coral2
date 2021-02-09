#include "RenderPass.h"
#include "Framebuffer.h"

using namespace coral;

void RenderPass::render(RenderQueue& queue)
{
    prepare();
    internalRender(queue);
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

void RenderPass::prepare()
{
    // retrieve inputs


    // output
    std::vector<std::string> colorOutputs;
    auto framebuffer = FramebufferManager::getFramebufferFor(colorOutputs);
    framebuffer->bind();
}
