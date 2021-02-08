#include "RenderPass.h"

using namespace coral;

void RenderPass::render(RenderQueue& queue)
{
    // bind output
    for (const RenderPassOutput& output : outputs)
    {
        switch (output.type)
        {
        case RenderPassOutputType::framebuffer:
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            break;

        default:
            break;
        }
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
