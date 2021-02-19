#pragma once

#include <gl/glew.h>

#include "Material.h"
#include "Property.h"
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
