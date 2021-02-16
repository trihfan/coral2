#ifndef ENCODER_MATERIAL_H
#define ENCODER_MATERIAL_H

#include "Material.h"
#include "Property.h"
#include "glad/glad.h"
#include <glm/glm.hpp>

namespace coral
{
    class EncoderMaterial : public Material
    {
    public:
        // construction
        EncoderMaterial();

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;

        // uniforms
        Property<GLuint> colorBuffer;
    };
}
#endif