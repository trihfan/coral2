#pragma once

#include "Material.h"

namespace coral
{
    class Resource;

    class PresentationMaterial : public Material
    {
    public:
        // construction
        PresentationMaterial();

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;
        virtual void setNode(Handle<Node> node) override;
    };
}
