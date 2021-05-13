#include "PipelineManager.h"
#include "base/ObjectFactory.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassManager.h"

using namespace coral;

DEFINE_SINGLETON(PipelineManager)

PipelineManager::PipelineManager()
{
}

void PipelineManager::release()
{
}

void PipelineManager::clear()
{
    for (const auto& pipeline : instance->pipelines)
    {
        pipeline.second->setDirty();
    }
    instance->pipelines.clear();
}

void PipelineManager::update()
{
}

ptr<Pipeline> PipelineManager::getPipelineByName(const std::string& name)
{
    auto it = std::find_if(instance->pipelines.begin(), instance->pipelines.end(), [&name](const auto& pair) { return pair.first.params.name == name; });
    if (it != instance->pipelines.end())
    {
        return it->second;
    }
    return nullptr;
}

ptr<Pipeline> PipelineManager::createPipeline(const std::string& renderpass, backend::BackendPipelineParams params)
{
    // Retrieve renderpass
    auto rd = RenderPassManager::getRenderPassByName(renderpass);
    if (!rd || !rd->getBackendRenderPass())
    {
        Logs(error) << "Can't find render pass " << renderpass;
        return nullptr;
    }

    params.renderpass = rd->getBackendRenderPass();
    auto pipeline = ObjectFactory::createWithName<Pipeline>(params.name, renderpass, params);
    instance->pipelines.push_back(std::make_pair(PipelineParams { renderpass, params }, pipeline));
    return pipeline;
}
