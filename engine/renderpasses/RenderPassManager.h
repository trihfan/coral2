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
#include <vector>
#include "utils/Singleton.h"

namespace coral
{
    class RenderPass;

    // Default renderpass ids used by the engine
    static const std::string defaultRenderPassName = "defaultRenderPass";

    // The renderpasses manager
    class RenderPassManager
    {
        MAKE_ENGINE_SINGLETON(RenderPassManager)
    public:
        static void invalidate();

        // add a render pass
		static void addRenderPass(std::shared_ptr<RenderPass> renderPass);

        // remove a render pass
		static void removeRenderPass(const std::string& name);

        // return the renderpass for the given id
        static std::shared_ptr<RenderPass> getRenderPass(const std::string& name);

        // update the graph
		static void update();

    private:
		RenderPassManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);

        // bake the render pass graph
        void bake();

    private:
		std::vector<std::shared_ptr<RenderPass>> renderPasses;
        std::vector<std::shared_ptr<RenderPass>> orderedRenderPasses;
    };
}
#endif