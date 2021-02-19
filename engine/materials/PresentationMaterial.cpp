#include "PresentationMaterial.h"
#include "Engine.h"
#include "backend/opengl/OpenglError.h"
#include "resources/Resource.h"
#include "resources/Shader.h"
#include "scene/camera/Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

PresentationMaterial::PresentationMaterial()
    : Material("presentation_material")
{
}

void PresentationMaterial::use(const RenderParameters& parameters)
{
}

void PresentationMaterial::setNode(Handle<Node> node)
{
}
