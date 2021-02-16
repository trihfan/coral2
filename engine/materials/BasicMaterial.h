#ifndef BASIC_MATERIAL_H
#define BASIC_MATERIAL_H

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

        // uniforms
        Property<glm::vec3> ambient;
        Property<glm::vec3> diffuse;
        Property<glm::vec3> specular;
        Property<float> shininess;
    };
}
#endif