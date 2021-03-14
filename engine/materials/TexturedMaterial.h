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

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;

        // uniforms
        std::vector<Handle<Resource>> diffuseTextures;
        std::vector<Handle<Resource>> specularTextures;
        std::vector<Handle<Resource>> normalTextures;
        std::vector<Handle<Resource>> heightTextures;

    protected:
        virtual Handle<Pipeline> createPipelineFor(const std::string& renderpass) override;
        virtual std::string getPipelineName() const override;
    };
}
