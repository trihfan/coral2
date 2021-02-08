#include "EncoderMaterial.h"
#include "Shader.h"

using namespace coral;

EncoderMaterial::EncoderMaterial() :
    Material("encoder_material")
{
}

void EncoderMaterial::use(const RenderParameters& parameters)
{
    Material::use(parameters);

    // color buffer
    //shader->set("model", *colorBuffer);
}