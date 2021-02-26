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

void PipelineManager::resize(int width, int height)
{
    for (auto& pair : instance->pipelines)
    {
        pair.second->resize(width, height);
    }
    instance->width = width;
    instance->height = height;
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
    auto pipeline = ObjectFactory::createWithName<Pipeline>(params.renderpass, params);
    pipeline->resize(instance->width, instance->height);
    instance->pipelines.push_back(std::make_pair(params, pipeline));
    return pipeline;
}
