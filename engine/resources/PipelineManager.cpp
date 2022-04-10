#include "PipelineManager.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassManager.h"
#include "utils/Logs.h"

using namespace coral;

Object<Pipeline> PipelineManager::getPipelineByName(const std::string& name)
{
    for (auto& pair : pipelines)
    {
        if (pair.first.name == name)
        {
            return pair.second;
        }
    }
    return nullptr;
}

Object<Pipeline> PipelineManager::createPipeline(PipelineParams params)
{
    // Retrieve renderpass
    auto renderPass = engine->renderPassManager->getRenderPassByName(params.renderpass);
    if (!renderPass || !renderPass->getBackendRenderPass())
    {
        Logs(error) << "Can't find render pass " << params.renderpass;
        return nullptr;
    }

    params.params.renderpass = renderPass->getBackendRenderPass();
    auto pipeline = ObjectFactory::createWithName<Pipeline>(params.params.name, params);
    pipelines.push_back(std::make_pair(params, pipeline));
    return pipeline;
}
