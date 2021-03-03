#include "BasicMaterial.h"
#include "AssetManager.h"
#include "Engine.h"
#include "renderpasses/RenderPassManager.h"
#include "resources/PipelineManager.h"
#include "scene/Node.h"
#include "scene/camera/Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

BasicMaterial::BasicMaterial()
{
    PipelineParams params;
    params.renderpass = defaultRenderPassName;

    params.vertexShaderFile = AssetManager::getAsset("basic_material.vert").url;
    params.fragmentShaderFile = AssetManager::getAsset("basic_material.frag").url;

    pipeline = PipelineManager::getPipeline(params);
}

void BasicMaterial::use(const RenderParameters& parameters)
{
    pipeline->setUniform("view", parameters.camera->getViewProjectionMatrix());
    pipeline->setUniform("viewPosition", parameters.camera->getWorldPosition());

    // Point lights
    size_t lightCount = std::min(parameters.lights.pointLights.size(), static_cast<size_t>(32));
    pipeline->setUniform("pointLightCount", static_cast<int>(lightCount));
    for (size_t i = 0; i < lightCount; i++)
    {
        const auto& light = parameters.lights.pointLights[i];
        std::string lightStr = "pointLights[" + std::to_string(i) + "]";
        pipeline->setUniform(lightStr + ".position", *light->position);
        pipeline->setUniform(lightStr + ".color", *light->color);
        pipeline->setUniform(lightStr + ".constant", *light->constant);
        pipeline->setUniform(lightStr + ".linear", *light->linear);
        pipeline->setUniform(lightStr + ".quadratic", *light->quadratic);
    }

    // Ambient light
    if (!parameters.lights.areaLights.empty())
    {
    }

    // Material
    pipeline->setUniform("material.color", *color);
    pipeline->setUniform("material.shininess", *shininess);
}

void BasicMaterial::setNode(Handle<Node> node)
{
    // Matrix
    pipeline->setUniform("model", node->getWorldMatrix());
}
