#include "RenderPass.h"

using namespace coral;

void RenderPass::render(RenderQueue& queue)
{
    internalRender(queue);
}

//*********************************************************************************
// RenderPassManager

DEFINE_SINGLETON(RenderPassManager)

void RenderPassManager::setDefaultRenderPass(std::shared_ptr<RenderPass> renderPass)
{
    instance->defaultRenderPass = renderPass;
}

void RenderPassManager::addRenderPass(unsigned int queueId, std::shared_ptr<RenderPass> renderPass)
{
    instance->renderPasses[queueId] = renderPass;
}

std::shared_ptr<RenderPass> RenderPassManager::getRenderPass(unsigned int queueId)
{
    auto it = instance->renderPasses.find(queueId);
    if (it != instance->renderPasses.end())
    {
        return it->second;
    }
    return instance->defaultRenderPass;
}

void RenderPassManager::update()
{
}

RenderPassManager::RenderPassManager(std::pmr::memory_resource* memory_resource)
{
}