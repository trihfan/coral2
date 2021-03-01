#pragma once

#include "Object.h"
#include "resources/Framebuffer.h"

namespace coral
{
    class Resource;
    class Pipeline;
    struct RenderQueue;
    struct RenderParameters;

    struct RenderPassResource
    {
        std::string name;
        backend::BackendFramebufferResourceRole role;
        backend::BackendResourceFormat format;
        int sampleCount;
    };

    class RenderPass : public Object
    {
    public:
        void invalidate();
        void prepare(const RenderParameters& parameters);

        // Render the content
        void render(RenderQueue& queue, const RenderParameters& parameters);

        // Inputs by name
        void addInput(const RenderPassResource& input);
        const std::vector<RenderPassResource>& getInputs() const;

        // Outputs by name
        void addOutput(const RenderPassResource& output);
        const std::vector<RenderPassResource>& getOutputs() const;

    protected:
        virtual void internalRender(RenderQueue& queue, const RenderParameters& parameters) = 0;

        // Loaded resouces
        std::vector<Handle<Resource>> inputResources;
        std::vector<Handle<Resource>> outputResources;

    private:
        Handle<Resource> getResource(const RenderPassResource& resource, const RenderParameters& parameters) const;

    private:
        // Render pass resources
        std::vector<RenderPassResource> inputs;
        std::vector<RenderPassResource> outputs;

        // The output framebuffer
        Handle<Framebuffer> framebuffer;
    };
}
