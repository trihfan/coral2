#include "RenderPassManager.h"

using namespace coral;

DEFINE_SINGLETON(RenderPassManager)

void RenderPassManager::setDefaultRenderPass(std::shared_ptr<RenderPass> renderPass)
{
    instance->defaultRenderPass = renderPass;
}

void RenderPassManager::addRenderPass(const std::string& id, std::shared_ptr<RenderPass> renderPass)
{
    instance->renderPasses[id] = renderPass;
    instance->orderedRenderPasses.clear();
}

void RenderPassManager::removeRenderPass(const std::string& id)
{
    auto it = instance->renderPasses.find(id);
    if (it != instance->renderPasses.end())
    {
        instance->renderPasses.erase(it);
        instance->orderedRenderPasses.clear();
    }
}

std::shared_ptr<RenderPass> RenderPassManager::getRenderPass(const std::string& id)
{
    auto it = instance->renderPasses.find(id);
    if (it != instance->renderPasses.end())
    {
        return it->second;
    }
    return instance->defaultRenderPass;
}

void RenderPassManager::update()
{
    // bake the graph
    if (instance->orderedRenderPasses.empty())
    {
        FramebufferManager::clear();
        instance->bake();
    }
}

RenderPassManager::RenderPassManager(std::shared_ptr<std::pmr::memory_resource> memory_resource)
{
}

void RenderPassManager::bake()
{
    // sort render passes
    for (auto& renderpass : renderPasses)
    {
        orderedRenderPasses.push_back(std::make_pair(renderpass.first, renderpass.second));
        renderpass.second->prepare();
    }
    orderedRenderPasses.push_back(std::make_pair(defaultRenderPassName, defaultRenderPass));
}