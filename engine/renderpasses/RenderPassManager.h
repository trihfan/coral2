#pragma once
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "Object.h"
#include "PropertyArray.h"

namespace coral
{
    class RenderPass;
    struct RenderParameters;

    // Default renderpass ids used by the engine
    inline const std::string defaultRenderPassName = "default";

    /**
     * @brief The RenderPassManager contains all renderpasses and organize them
     * dpeending on their input / outputs
     */
    class RenderPassManager
    {
    public:
        // Construction
        RenderPassManager();

        // Properties
        PropertyArray<std::shared_ptr<RenderPass>> renderPasses;

        // Helpers
        std::shared_ptr<RenderPass> getRenderPassByName(const std::string& name);

        // Invalidate the render graph
        void invalidate();

        // Update the render graph
        void update();

        // Return the renderpasses ordered by render order
        const std::vector<std::shared_ptr<RenderPass>>& getOrderedRenderPasses();

    private:
        // Ordered renderpasses by render priority
        std::vector<std::shared_ptr<RenderPass>> orderedRenderPasses;

        // Bake the render graph
        void bake();
    };
}
