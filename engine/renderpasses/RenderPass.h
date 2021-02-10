#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "Object.h"
#include "resources/Framebuffer.h"

namespace coral
{
    struct RenderQueue;

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
        // retrieve actual resources (if existing or allocate it)
        void clear();
        void prepare();

        // render the content
        void render(RenderQueue& queue);

        void addInput(const std::string& input);
        const std::vector<std::string>& getInputs() const;

        void addOutput(const RenderPassResource& output);
        const std::vector<RenderPassResource>& getOutputs() const;

    protected:
        // render implementation
        virtual void internalRender(RenderQueue& queue) = 0;

    private:
        // render pass resources
        std::vector<std::string> inputs;
        std::vector<RenderPassResource> outputs;

        // The output framebuffer
        std::shared_ptr<Framebuffer> framebuffer;

        
    };
}
#endif