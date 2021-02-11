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

    /*
     * @brief The RenderPassManager contains all renderpasses and organize them
     * dpeending on their input / outputs
     */
    class RenderPassManager
    {
        MAKE_ENGINE_SINGLETON(RenderPassManager)
    public:
        /*
         * @brief Invalidate the render graph
         */
        static void invalidate();

        /*
         * @brief Add a render pass
         */
		static void addRenderPass(std::shared_ptr<RenderPass> renderPass);

        /*
         * @brief Remove a render pass
         */
		static void removeRenderPass(const std::string& name);

        /*
         * @brief Return the renderpass for the given id
         */
        static std::shared_ptr<RenderPass> getRenderPass(const std::string& name);

    private:
        /*
         * @brief Constructor
         */
		RenderPassManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);

        /*
         * @brief Update if needed the graph
         */
		static void update();

        /*
         * @brief Bake the render graph
         */
        void bake();

    private:
        // The renderpasses list
		std::vector<std::shared_ptr<RenderPass>> renderPasses;

        // Ordered renderpasses by render priority
        std::vector<std::shared_ptr<RenderPass>> orderedRenderPasses;
    };
}
#endif