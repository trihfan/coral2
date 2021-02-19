#include "BasicMaterial.h"
#include "Engine.h"
#include "resources/Shader.h"
#include "scene/Node.h"
#include "scene/camera/Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

BasicMaterial::BasicMaterial()
    : Material("basic_material")
{
}

void BasicMaterial::use(const RenderParameters& parameters)
{
    shader->setMat4("view", parameters.camera->getViewProjectionMatrix());
    shader->setVec3("viewPosition", parameters.camera->getWorldPosition());

    // Point lights
    size_t lightCount = std::min(parameters.lights.pointLights.size(), static_cast<size_t>(32));
    shader->setInt("pointLightCount", static_cast<int>(lightCount));
    for (size_t i = 0; i < lightCount; i++)
    {
        const auto& light = parameters.lights.pointLights[i];
        std::string lightStr = "pointLights[" + std::to_string(i) + "]";
        shader->setVec3(lightStr + ".position", *light->position);
        shader->setVec3(lightStr + ".color", *light->color);
        shader->setFloat(lightStr + ".constant", *light->constant);
        shader->setFloat(lightStr + ".linear", *light->linear);
        shader->setFloat(lightStr + ".quadratic", *light->quadratic);
    }

    // Ambient light
    if (!parameters.lights.areaLights.empty())
    {
    }

    // Material
    shader->setVec3("material.color", *color);
    shader->setFloat("material.shininess", *shininess);
}

void BasicMaterial::setNode(Handle<Node> node)
{
    // Matrix
    shader->setMat4("model", node->getWorldMatrix());
}
