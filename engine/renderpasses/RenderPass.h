#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "Object.h"
#include "resources/Framebuffer.h"

namespace coral
{
    class Resource;
    struct RenderQueue;
    struct RenderParameters;

    struct RenderPassResource
    {
        std::string name;
        ResourceRole role;
        GLint internalFormat;
        GLenum format;
        GLenum type;
        int sampleCount;
    };

    class RenderPass : public Object
    {
    public:
        void invalidate();
        void prepare(const RenderParameters& parameters);

        // render the content
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
        std::vector<std::shared_ptr<Resource>> inputResources;
        std::vector<std::shared_ptr<Resource>> outputResources;

    private:
        std::shared_ptr<Resource> getResource(const RenderPassResource& resource, const RenderParameters& parameters) const;

    private:
        // render pass resources
        std::vector<RenderPassResource> inputs;
        std::vector<RenderPassResource> outputs;

        // The output framebuffer
        std::shared_ptr<Framebuffer> framebuffer;
    };
}
#endif