#pragma once

#include "Material.h"
#include <glm/glm.hpp>

namespace coral
{

    class BasicMaterial : public Material
    {
    public:
        BasicMaterial(const std::vector<std::string>& renderQueueTags);

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;

        // uniforms
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

    protected:
        virtual Handle<Pipeline> createPipelineFor(const std::string& renderpass) override;
        virtual std::string getPipelineName() const override;
    };
}
