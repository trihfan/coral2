#pragma once

#include "Material.h"
#include "resources/Resource.h"
#include <vector>

namespace coral
{
    class TexturedMaterial : public Material
    {
    public:
        TexturedMaterial(const std::vector<std::string>& renderQueueTags);

        // Return the attributes of the material
        virtual std::vector<ShaderAttribute> getAttributes() const override;

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;
        virtual void setNode(Handle<Node> node) override;

        // uniforms
        std::vector<Handle<Resource>> diffuseTextures;
        std::vector<Handle<Resource>> specularTextures;
        std::vector<Handle<Resource>> normalTextures;
        std::vector<Handle<Resource>> heightTextures;

    protected:
        virtual Handle<Pipeline> getPipelineFor(const std::string& renderpass) override;
    };
}
