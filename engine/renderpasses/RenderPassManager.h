#ifndef RENDERPASSMANAGER_H
#define RENDERPASSMANAGER_H

#include "memory_resource.h"
#include "utils/Singleton.h"
#include <map>
#include <memory>
#include <vector>

namespace coral
{
    class RenderPass;
    struct RenderParameters;

    // Default renderpass ids used by the engine
    static const std::string defaultRenderPassName = "default";

    /**
     * @brief The RenderPassManager contains all renderpasses and organize them
     * dpeending on their input / outputs
     */
    class RenderPassManager
    {
        MAKE_SINGLETON(RenderPassManager)
    public:
        /**
         * @brief Invalidate the render graph
         */
        static void invalidate();

        /**
         * @brief Update if needed the graph
         */
        static void update(const RenderParameters& parameters);

        /**
         * @brief Add a render pass
         */
        static void addRenderPass(std::shared_ptr<RenderPass> renderPass);

        /**
         * @brief Remove a render pass
         */
        static void removeRenderPass(const std::string& name);

        /**
         * @brief Return the renderpass for the given id
         */
        static std::shared_ptr<RenderPass> getRenderPass(const std::string& name);

        /**
         * @brief Return the renderpasses ordered by render order
         */
        static const std::vector<std::shared_ptr<RenderPass>>& getOrderedRenderPasses();

    private:
        /**
         * @brief Constructor
         */
        RenderPassManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);

        /**
         * @brief Bake the render graph
         */
        void bake(const RenderParameters& parameters);

    private:
        // The renderpasses list
        std::vector<std::shared_ptr<RenderPass>> renderPasses;

        // Ordered renderpasses by render priority
        std::vector<std::shared_ptr<RenderPass>> orderedRenderPasses;
    };
}
#endif