#include "PipelineManager.h"
#include "base/ObjectFactory.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassManager.h"

using namespace coral;

void PipelineManager::clear()
{
    for (const auto& pipeline : instance->pipelines)
    {
        pipeline.second->setDirty();
    }
    instance->pipelines.clear();
}

Handle<Pipeline> PipelineManager::getPipelineByName(const std::string& name)
{
    auto it = std::find_if(instance->pipelines.begin(), instance->pipelines.end(), [&name](const auto& pair) { return pair.first.params.name == name; });
    if (it != instance->pipelines.end())
    {
        return it->second;
    }
    return nullptr;
}

Handle<Pipeline> PipelineManager::createPipeline(PipelineParams params)
{
    // Retrieve renderpass
    auto renderPass = RenderPassManager::getRenderPassByName(params.renderpass);
    if (!renderPass || !renderPass->getBackendRenderPass())
    {
        Logs(error) << "Can't find render pass " << params.renderpass;
        return nullptr;
    }

    params.params.renderpass = renderPass->getBackendRenderPass();
    auto pipeline = ObjectFactory::createWithName<Pipeline>(params.params.name, params);
    instance->pipelines.push_back(std::make_pair(params, pipeline));
    return pipeline;
}
