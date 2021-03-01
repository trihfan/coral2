#include "PresentationMaterial.h"
#include "resources/AssetManager.h"
#include "resources/PipelineManager.h"

using namespace coral;

PresentationMaterial::PresentationMaterial()
{
    PipelineParams params;
    params.renderpass = "presentationRenderPass";

    auto shader = AssetManager::getShader("presentation_material");
    params.vertexShaderFile = shader.vertexFile;
    params.fragmentShaderFile = shader.fragmentFile;

    params.depthTest = false;

    pipeline = PipelineManager::getPipeline(params);
}

void PresentationMaterial::use(const RenderParameters& parameters)
{
}

void PresentationMaterial::setNode(Handle<Node> node)
{
}
