#ifndef RENDERPASSMANAGER_H
#define RENDERPASSMANAGER_H

#ifdef __APPLE__
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#else
    #include <memory_resource>
#endif
#include <map>
#include <memory>
#include "utils/Singleton.h"

namespace coral
{
    class RenderPass;

    // Default renderpass ids used by the engine
    enum DefaultRenderPassId : uint16_t
    {
        defaultRenderPass = 1000,
        transparentRenderPass = 2000
    };

    // The renderpasses manager
    class RenderPassManager
    {
        MAKE_ENGINE_SINGLETON(RenderPassManager)
    public:
        // set the renderpass to use when no renderpass is registered for a given queue id
        static void setDefaultRenderPass(std::shared_ptr<RenderPass> renderPass);

        // add a render pass
		static void addRenderPass(uint16_t id, std::shared_ptr<RenderPass> renderPass);

        // remove a render pass
		static void removeRenderPass(uint16_t id);

        // return the renderpass for the given id
        static std::shared_ptr<RenderPass> getRenderPass(uint16_t id);

        // update the graph
		static void update();

    private:
		RenderPassManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);

        // bake the render pass graph
        void bake();

    private:
		std::shared_ptr<RenderPass> defaultRenderPass;
		std::map<uint16_t, std::shared_ptr<RenderPass>> renderPasses;
        std::vector<std::pair<uint16_t, std::shared_ptr<RenderPass>>> orderedRenderPasses;
    };
}
#endif