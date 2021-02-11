#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "Object.h"
#include "resources/Framebuffer.h"

namespace coral
{
    class Resource;
    struct RenderQueue;

    struct RenderPassInput
    {
        std::string name;
    };

    struct RenderPassOutput
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
        // retrieve actual resources (if existing or allocate it)
        void clear();
        void prepare();

        // render the content
        void render(RenderQueue& queue);

        // Inputs by name
        void addInput(const RenderPassInput& input);
        const std::vector<RenderPassInput>& getInputs() const;

        // Outputs by name
        void addOutput(const RenderPassOutput& output);
        const std::vector<RenderPassOutput>& getOutputs() const;

    protected:
        // render implementation
        virtual void internalRender(RenderQueue& queue) = 0;

    private:
        // render pass resources
        std::vector<RenderPassInput> inputs;
        std::vector<RenderPassOutput> outputs;
        std::vector<std::shared_ptr<Resource>> resources;

        // The output framebuffer
        std::shared_ptr<Framebuffer> framebuffer;
    };
}
#endif