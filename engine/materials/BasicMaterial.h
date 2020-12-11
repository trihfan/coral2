#ifndef BASIC_MATERIAL_H
#define BASIC_MATERIAL_H

#include <glm/glm.hpp>
#include "Material.h"

namespace coral
{
    class BasicMaterial : public Material
    {
    public:
        // construction
        BasicMaterial() = default;
        virtual ~BasicMaterial() = default;

        // setup the shader and its parameters
        virtual void use() override;

    private:
        // basic material parameters
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };
}
#endif