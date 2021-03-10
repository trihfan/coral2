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
}

std::vector<ShaderAttribute> BasicMaterial::getAttributes() const
{
    return {
        ShaderAttribute { ShaderAttributeType::position, 0 },
        ShaderAttribute { ShaderAttributeType::normal, 1 }
    };
}

void BasicMaterial::use(const RenderParameters& parameters)
{
    setupLights(parameters);

    getPipeline()->setUniform("view", parameters.camera->getViewProjectionMatrix());
    getPipeline()->setUniform("viewPosition", parameters.camera->getPosition());

    // Material
    getPipeline()->setUniform("material.ambient", ambient);
    getPipeline()->setUniform("material.diffuse", diffuse);
    getPipeline()->setUniform("material.specular", specular);
    getPipeline()->setUniform("material.shininess", shininess);
}

void BasicMaterial::setNode(Handle<Node> node)
{
    // Matrix
    getPipeline()->setUniform("model", node->getMatrix());
}

Handle<Pipeline> BasicMaterial::getPipelineFor(const std::string& renderpass)
{
    PipelineParams params;
    params.renderpass = renderpass;
    params.params.name = "basic_material";
    params.params.vertexShaderFile = AssetManager::getShader("basic_material", ShaderType::vertex).asset.url;
    params.params.fragmentShaderFile = AssetManager::getShader("basic_material", ShaderType::fragment).asset.url;
    return PipelineManager::getPipeline(params);
}
