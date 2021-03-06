#include "Material.h"
#include "RenderParameters.h"

using namespace coral;

void Material::setupLights(const RenderParameters& parameters)
{
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
}
