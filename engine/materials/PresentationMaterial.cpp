#include "PresentationMaterial.h"
#include "resources/PipelineManager.h"
#include "resources/ResourceManager.h"

using namespace coral;

PresentationMaterial::PresentationMaterial()
{
    PipelineParams params;
    params.renderpass = "presentationRenderPass";

    auto shader = ResourceManager::getShader("presentation_material");
    params.vertexShaderFile = shader.vertexFile;
    params.fragmentShaderFile = shader.fragmentFile;

    pipeline = PipelineManager::getPipeline(params);
}

void PresentationMaterial::use(const RenderParameters& parameters)
{
}

void PresentationMaterial::setNode(Handle<Node> node)
{
}
