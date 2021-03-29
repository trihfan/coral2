#pragma once

#include "base/Object.h"
#include "base/Ptr.h"
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
        static void addRenderPass(ptr<RenderPass> renderPass);

        /**
         * @brief Remove a render pass
         */
        static void removeRenderPass(const std::string& name);

        /**
         * @brief Return the renderpass for the given id
         */
        static ptr<RenderPass> getRenderPassByName(const std::string& name);

        /**
         * @brief Return the renderpasses ordered by render order
         */
        static const std::vector<ptr<RenderPass>>& getOrderedRenderPasses();

    private:
        RenderPassManager() = default;

        /**
         * @brief Bake the render graph
         */
        void bake(const RenderParameters& parameters);

    private:
        // The renderpasses list
        std::vector<ptr<RenderPass>> renderPasses;

        // Ordered renderpasses by render priority
        std::vector<ptr<RenderPass>> orderedRenderPasses;
    };
}
