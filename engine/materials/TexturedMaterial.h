#pragma once

#include "Material.h"
#include "Property.h"
#include "resources/Resource.h"

namespace coral
{
    class TexturedMaterial : public Material
    {
    public:
        TexturedMaterial();

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;
        virtual void setNode(Handle<Node> node) override;

        // uniforms
        PropertyArray<Handle<Resource>> diffuseTextures;
        PropertyArray<Handle<Resource>> specularTextures;
        PropertyArray<Handle<Resource>> normalTextures;
        PropertyArray<Handle<Resource>> heightTextures;
    };
}
