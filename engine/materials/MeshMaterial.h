#pragma once

#include "Material.h"
#include "Property.h"
#include "resources/Resource.h"

namespace coral
{
    class MeshMaterial : public Material
    {
    public:
        MeshMaterial();

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;
        virtual void setNode(Handle<Node> node) override;

        // uniforms
        Property<Handle<Resource>> textureDiffuse;
    };
}
