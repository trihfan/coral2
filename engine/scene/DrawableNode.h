#pragma once
#include "Node.h"

namespace coral
{
    struct RenderParameters;
    class Material;

    // A node represent an drawable item in the scene
    class DrawableNode : public Node
    {
    public:
        // Construction
        DrawableNode() = default;
        virtual ~DrawableNode() override = default;

        // Properties
        Property<Handle<Material>> material;

        // Update and draw
        virtual void update() override;
        virtual void draw() = 0;
    };
}
