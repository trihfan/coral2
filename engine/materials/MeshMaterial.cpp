#include "MeshMaterial.h"
#include "AssetManager.h"
#include "Engine.h"
#include "renderpasses/RenderPassManager.h"
#include "resources/PipelineManager.h"
#include "scene/Node.h"
#include "scene/camera/Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

MeshMaterial::MeshMaterial()
{
    PipelineParams params { "mesh_material" };
    params.renderpass = defaultRenderPassName;

    params.vertexShaderFile = AssetManager::getShader("mesh_material", ShaderType::vertex).asset.url;
    params.fragmentShaderFile = AssetManager::getShader("mesh_material", ShaderType::fragment).asset.url;

    pipeline = PipelineManager::getPipeline(params);
}

void MeshMaterial::use(const RenderParameters& parameters)
{
    pipeline->setUniform("view", parameters.camera->getViewProjectionMatrix());
    pipeline->setUniform("viewPosition", parameters.camera->getWorldPosition());

    if (textureDiffuse.get())
    {
        textureDiffuse->bind(0);
        pipeline->setUniform("texture_diffuse1", 0);
    }
}

void MeshMaterial::setNode(Handle<Node> node)
{
    pipeline->setUniform("model", node->getWorldMatrix());
}
