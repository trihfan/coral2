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
    params.name = "basic_material";
    params.renderpass = defaultRenderPassName;
    params.vertexShaderFile = AssetManager::getShader("basic_material", ShaderType::vertex).asset.url;
    params.fragmentShaderFile = AssetManager::getShader("basic_material", ShaderType::fragment).asset.url;

    pipeline = PipelineManager::getPipeline(params);
}

void BasicMaterial::use(const RenderParameters& parameters)
{
    setupLights(parameters);

    pipeline->setUniform("view", parameters.camera->getViewProjectionMatrix());
    pipeline->setUniform("viewPosition", parameters.camera->getWorldPosition());

    // Material
    pipeline->setUniform("material.ambient", *ambient);
    pipeline->setUniform("material.diffuse", *diffuse);
    pipeline->setUniform("material.specular", *specular);
    pipeline->setUniform("material.shininess", *shininess);
}

void BasicMaterial::setNode(Handle<Node> node)
{
    // Matrix
    pipeline->setUniform("model", node->getWorldMatrix());
}
