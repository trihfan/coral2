#pragma once

#include "Node.h"
#include "renderpasses/RenderPassManager.h"
#include <memory>
#include <vector>

namespace coral
{
    struct RenderParameters;
    class Material;

    // A node represent an drawable item in the scene
    class DrawableNode : public Node
    {
    public:
        // construction
        DrawableNode(Handle<Node> parent = nullptr, const std::vector<std::string>& renderQueueTags = { coral::defaultRenderPassName });
        virtual ~DrawableNode() override = default;

        // meta
        bool isDrawable() const override;

        // render queue
        void addRenderQueueTag(const std::string& renderQueueId);
        void removeRenderQueueTag(const std::string& renderQueueId);
        bool isTagForRenderQueue(const std::string& renderQueueId) const;
        const std::vector<std::string>& getRenderQueueTags() const;

        // material
        void setMaterial(Handle<Material> material);
        Handle<Material> getMaterial() const;

        // draw
        virtual void draw(const RenderParameters& parameters) = 0;

    protected:
        Handle<Material> material;
        std::vector<std::string> renderQueueTags;
    };
}
