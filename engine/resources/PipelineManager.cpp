#include "PipelineManager.h"
#include "ObjectFactory.h"
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

Handle<Pipeline> PipelineManager::getPipeline(const PipelineParams& params)
{
    // Find existing
    auto it = std::find_if(instance->pipelines.begin(), instance->pipelines.end(), [&params](const auto& pair) { return pair.first == params; });
    if (it != instance->pipelines.end())
    {
        return it->second;
    }

    // Create new pipeline
    auto pipeline = ObjectFactory::createWithName<Pipeline>(params.params.name, params);
    instance->pipelines.push_back(std::make_pair(params, pipeline));
    return pipeline;
}
