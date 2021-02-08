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

    class RenderPass : public Object
    {
    public:
        void render(RenderQueue& queue);

    protected:
        virtual void internalRender(RenderQueue& queue) = 0;
    };

    // The renderpasses manager
    class RenderPassManager
    {
        MAKE_ENGINE_SINGLETON(RenderPassManager)
    public:
        static void setDefaultRenderPass(std::shared_ptr<RenderPass> renderPass);
		static void addRenderPass(unsigned int queueId, std::shared_ptr<RenderPass> renderPass);
        static std::shared_ptr<RenderPass> getRenderPass(unsigned int queueId);

		static void update();

    private:
		RenderPassManager(std::pmr::memory_resource* memory_resource);

    private:
		std::shared_ptr<RenderPass> defaultRenderPass;
		std::map<unsigned int, std::shared_ptr<RenderPass>> renderPasses;
    };
}
#endif