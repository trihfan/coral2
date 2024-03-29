#include "Material.h"
#include "RenderParameters.h"
#include "resources/PipelineManager.h"
#include "scene/camera/Camera.h"
#include "scene/light/PointLight.h"

using namespace coral;

Material::Material(const std::vector<std::string>& renderQueueTags)
    : renderQueueTags(renderQueueTags)
{
    if (renderQueueTags.empty())
    {
        Logs(error) << "No given renderpass for the material";
    }
}

Handle<Pipeline> Material::getPipeline() const
{
    return pipeline;
}

void Material::setupLights(const RenderParameters& parameters)
{
    pipeline->setUniform("viewPosition", parameters.camera->getWorldPosition());

    // Point lights
    size_t lightCount = std::min(parameters.lights.pointLights.size(), static_cast<size_t>(32));
    pipeline->setUniform("pointLightCount", static_cast<int>(lightCount));
    for (size_t i = 0; i < lightCount; i++)
    {
        const auto& light = parameters.lights.pointLights[i];
        std::string lightStr = "pointLights[" + std::to_string(i) + "]";
        pipeline->setUniform(lightStr + ".position", light->getWorldPosition());
        pipeline->setUniform(lightStr + ".color", light->color);
        pipeline->setUniform(lightStr + ".constant", light->constant);
        pipeline->setUniform(lightStr + ".linear", light->linear);
        pipeline->setUniform(lightStr + ".quadratic", light->quadratic);
    }
}

void Material::use(const RenderParameters& parameters)
{
    pipeline->setUniform("transform.projectionMatrix", parameters.camera->getProjectionMatrix());
    pipeline->setUniform("transform.viewMatrix", parameters.camera->getViewMatrix());

    if (lighting)
    {
        setupLights(parameters);
    }
}

void Material::init()
{
    Object::init();
    invalidatePipeline();
}

void Material::invalidatePipeline()
{
    pipeline = PipelineManager::getPipelineByName(getPipelineName());
    if (!pipeline)
    {
        pipeline = createPipelineFor(renderQueueTags[0]);
    }
}

void Material::setNode(Handle<Node> node)
{
    pipeline->setUniform("transform.modelMatrix", node->transform().getMatrix());
}

void Material::enableLighting()
{
    lighting = true;
}
