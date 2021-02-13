#include "PresentationMaterial.h"
#include "Engine.h"
#include "resources/Resource.h"
#include "resources/Shader.h"
#include "scene/camera/Camera.h"
#include "utils/Error.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

PresentationMaterial::PresentationMaterial()
    : Material("presentation_material")
{
}

void PresentationMaterial::use(const RenderParameters& parameters)
{
}