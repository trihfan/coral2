#include "PresentationMaterial.h"
#include "AssetManager.h"
#include "resources/PipelineManager.h"

using namespace coral;

PresentationMaterial::PresentationMaterial()
{
    PipelineParams params;
    params.renderpass = "presentationRenderPass";

    params.vertexShaderFile = AssetManager::getAsset("presentation_material.vert").url;
    params.fragmentShaderFile = AssetManager::getAsset("presentation_material.frag").url;

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
