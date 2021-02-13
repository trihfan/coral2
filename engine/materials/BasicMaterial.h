#ifndef BASIC_MATERIAL_H
#define BASIC_MATERIAL_H

#include <glm/glm.hpp>
#include "Property.h"
#include "Material.h"

namespace coral
{
    class BasicMaterial : public Material
    {
    public:
        // construction
        BasicMaterial();
        virtual ~BasicMaterial() = default;

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