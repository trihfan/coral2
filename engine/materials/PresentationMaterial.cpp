#include "PresentationMaterial.h"
#include "AssetManager.h"
#include "resources/PipelineManager.h"

using namespace coral;

PresentationMaterial::PresentationMaterial()
{
    PipelineParams params { "presentation_material" };
    params.renderpass = "presentationRenderPass";

    params.vertexShaderFile = AssetManager::getShader("presentation_material", ShaderType::vertex).asset.url;
    params.fragmentShaderFile = AssetManager::getShader("presentation_material", ShaderType::fragment).asset.url;

    params.depthTest = false;

    pipeline = PipelineManager::getPipeline(params);
}

void PresentationMaterial::use(const RenderParameters&)
{
    pipeline->setUniform("backbuffer", 0);
}

void PresentationMaterial::setNode(Handle<Node>)
{
}
