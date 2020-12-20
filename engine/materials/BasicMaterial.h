#ifndef BASIC_MATERIAL_H
#define BASIC_MATERIAL_H

#include <glm/glm.hpp>
#include "Material.h"

namespace coral
{
    class BasicMaterial : public Material
    {
        DECLARE_TYPE(BasicMaterial)
    public:
        // construction
        BasicMaterial();
        virtual ~BasicMaterial() = default;

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;

        // uniforms
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };
}
#endif