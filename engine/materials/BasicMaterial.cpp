#include "BasicMaterial.h"
#include "AssetManager.h"
#include "Engine.h"
#include "renderpasses/RenderPassManager.h"
#include "resources/PipelineManager.h"
#include "scene/Node.h"
#include "scene/camera/Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

BasicMaterial::BasicMaterial(const std::vector<std::string>& renderQueueTags)
    : Material(renderQueueTags)
{
    enableLighting();
}

void BasicMaterial::use(const RenderParameters& parameters)
{
    Material::use(parameters);
    getPipeline()->setUniform("material.ambient", ambient);
    getPipeline()->setUniform("material.diffuse", diffuse);
    getPipeline()->setUniform("material.specular", specular);
    getPipeline()->setUniform("material.shininess", shininess);
}

Handle<Pipeline> BasicMaterial::createPipelineFor(const std::string& renderpass)
{
    PipelineParams params;
    params.renderpass = renderpass;
    params.params.name = getPipelineName();
    params.params.vertexShaderFile = AssetManager::getShader("basic_material", ShaderType::vertex).asset.url;
    params.params.fragmentShaderFile = AssetManager::getShader("basic_material", ShaderType::fragment).asset.url;
    return PipelineManager::createPipeline(params);
}

std::string BasicMaterial::getPipelineName() const
{
    return "BasicMaterialPipeline";
}
