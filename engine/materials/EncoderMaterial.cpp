#include "EncoderMaterial.h"
#include "resources/Shader.h"

using namespace coral;

EncoderMaterial::EncoderMaterial()
    : Material("encoder_material")
{
}

void EncoderMaterial::use(const RenderParameters& parameters)
{
    // color buffer
    //shader->set("model", *colorBuffer);
}