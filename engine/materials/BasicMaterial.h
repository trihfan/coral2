#pragma once

#include "Material.h"
#include "Property.h"
#include <glm/glm.hpp>

namespace coral
{

    class BasicMaterial : public Material
    {
    public:
        BasicMaterial();

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;
        virtual void setNode(Handle<Node> node) override;

        // uniforms
        Property<glm::vec3> color;
        Property<float> shininess;
    };
}
