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
        DrawableNode() = default;
        virtual ~DrawableNode() override = default;

        // meta
        bool isDrawable() const override;

        // material
        void setMaterial(ptr<Material> material);
        ptr<Material> getMaterial() const;

        virtual void update(const NodeUpdateParameters& parameters) override;

        // draw
        virtual void draw(const RenderParameters& parameters) = 0;

    protected:
        ptr<Material> material;
    };
}
