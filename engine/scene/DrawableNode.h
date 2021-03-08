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
        DrawableNode(Handle<Node> parent = nullptr);
        virtual ~DrawableNode() override = default;

        // meta
        bool isDrawable() const override;

        // material
        void setMaterial(Handle<Material> material);
        Handle<Material> getMaterial() const;

        virtual void update() override;

        // draw
        virtual void draw(const RenderParameters& parameters) = 0;

    protected:
        Handle<Material> material;
    };
}
