#include "RenderPassManager.h"
#include "RenderPass.h"
#include "RenderPassFramebufferManager.h"
#include "RenderPassResourceManager.h"
#include "scene/Scene.h"

using namespace coral;

DEFINE_SINGLETON(RenderPassManager)

void RenderPassManager::release()
{
}

void RenderPassManager::addRenderPass(Handle<RenderPass> renderPass)
{
    instance->renderPasses.push_back(renderPass);
    instance->orderedRenderPasses.clear();
}

void RenderPassManager::removeRenderPass(const std::string& name)
{
    auto it = std::find_if(instance->renderPasses.begin(), instance->renderPasses.end(), [name](const Handle<RenderPass> renderpass) { return renderpass->getName() == name; });
    if (it != instance->renderPasses.end())
    {
        instance->renderPasses.erase(it);
        instance->orderedRenderPasses.clear();
    }
}

Handle<RenderPass> RenderPassManager::getRenderPassByName(const std::string& name)
{
    auto it = std::find_if(instance->renderPasses.begin(), instance->renderPasses.end(), [name](const Handle<RenderPass> renderpass) { return renderpass->getName() == name; });
    if (it != instance->renderPasses.end())
    {
        return *it;
    }
    return nullptr;
}

void RenderPassManager::update(const RenderParameters& parameters)
{
    // bake the graph
    if (instance->orderedRenderPasses.empty())
    {
        instance->bake(parameters);
    }
}

void RenderPassManager::bake(const RenderParameters& parameters)
{
    // clear resources
    RenderPassFramebufferManager::clear();
    RenderPassResourceManager::clear();

    // sort render passes
    for (auto renderpass : renderPasses)
    {
        orderedRenderPasses.push_back(renderpass);
        renderpass->prepare(parameters);
    }
}

void RenderPassManager::invalidate()
{
    instance->orderedRenderPasses.clear();
}

const std::vector<Handle<RenderPass>>& RenderPassManager::getOrderedRenderPasses()
{
    return instance->orderedRenderPasses;
}
