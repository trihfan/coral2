#ifndef RENDERPASS_H
#define RENDERPASS_H

#ifdef __APPLE__
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#else
    #include <memory_resource>
#endif
#include <map>
#include <memory>
#include "Object.h"
#include "utils/Singleton.h"

namespace coral
{
    struct RenderQueue;

    enum class RenderPassInputType
    {
        color, depth
    };

    enum class RenderPassOutputType
    {
        framebuffer
    };

    struct RenderPassInput
    {
        RenderPassInputType type;
        std::string name;
    };

    struct RenderPassOutput
    {
        RenderPassOutputType type;
        std::string name;
    };

    class RenderPass : public Object
    {
    public:
        void render(RenderQueue& queue);

        void addInput(const RenderPassInput& input);
        const std::vector<RenderPassInput>& getInputs() const;

        void addOutput(const RenderPassOutput& output);
        const std::vector<RenderPassOutput>& getOutputs() const;

    protected:
        virtual void internalRender(RenderQueue& queue) = 0;

    private:
        std::vector<RenderPassInput> inputs;
        std::vector<RenderPassOutput> outputs;
    };
}
#endif