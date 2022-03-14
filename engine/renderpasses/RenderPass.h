#pragma once

#include "BackendRenderPass.h"
#include "base/Object.h"
#include "resources/Framebuffer.h"
#include <memory>

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

        backend::BackendRenderPass* getBackendRenderPass() const;

    protected:
        virtual void internalRender(RenderQueue& queue, const RenderParameters& parameters) = 0;

        // Loaded resouces
        std::vector<Handle<Resource>> inputResources;
        std::vector<Handle<Resource>> outputResources;

    private:
        // Render pass resources
        std::vector<RenderPassResource> inputs;
        std::vector<RenderPassResource> outputs;

        // The output framebuffer
        Handle<Framebuffer> framebuffer;
        std::unique_Handle<backend::BackendRenderPass> backendRenderPass;

        Handle<Resource> getResource(const RenderPassResource& resource, const RenderParameters& parameters) const;
    };
}
