#ifndef ENCODER_MATERIAL_H
#define ENCODER_MATERIAL_H

#include <glm/glm.hpp>
#include "Material.h"
#include "Property.h"

namespace coral
{
    class EncoderMaterial : public Material
    {
    public:
        // construction
        EncoderMaterial();
        virtual ~EncoderMaterial() = default;

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;

        // uniforms
        Property<GLuint> colorBuffer;
    };
}
#endif