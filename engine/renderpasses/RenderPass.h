#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "Object.h"

namespace coral
{
    struct RenderQueue;

    struct RenderPassResource
    {
        std::string name;
    };

    class RenderPass : public Object
    {
    public:
        void render(RenderQueue& queue);

        void addInput(const RenderPassResource& input);
        const std::vector<RenderPassResource>& getInputs() const;

        void addOutput(const RenderPassResource& output);
        const std::vector<RenderPassResource>& getOutputs() const;

    protected:
        // inputs
        // outputs

        // render implementation
        virtual void internalRender(RenderQueue& queue) = 0;

    private:
        // render pass resources
        std::vector<RenderPassResource> inputs;
        std::vector<RenderPassResource> outputs;

        // retrieve actual resources (if existing or allocate it)
        void prepare();
    };
}
#endif