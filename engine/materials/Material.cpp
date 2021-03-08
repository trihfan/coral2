#include "Material.h"
#include "RenderParameters.h"
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
    // Point lights
    size_t lightCount = std::min(parameters.lights.pointLights.size(), static_cast<size_t>(32));
    pipeline->setUniform("pointLightCount", static_cast<int>(lightCount));
    for (size_t i = 0; i < lightCount; i++)
    {
        const auto& light = parameters.lights.pointLights[i];
        std::string lightStr = "pointLights[" + std::to_string(i) + "]";
        pipeline->setUniform(lightStr + ".position", light->getPosition());
        pipeline->setUniform(lightStr + ".color", light->color);
        pipeline->setUniform(lightStr + ".constant", light->constant);
        pipeline->setUniform(lightStr + ".linear", light->linear);
        pipeline->setUniform(lightStr + ".quadratic", light->quadratic);
    }

    // Ambient light
    if (!parameters.lights.areaLights.empty())
    {
    }
}

void Material::init()
{
    Object::init();
    pipeline = getPipelineFor(renderQueueTags[0]);
}

void Material::update()
{
    Object::update();

    // Check the pipeline (todo)
    if (!pipeline || pipeline->isDirty())
    {
        pipeline = getPipelineFor(renderQueueTags[0]);
    }
}
