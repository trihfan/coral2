#include "RenderPassManager.h"
#include "RenderPass.h"
#include "RenderPassFramebufferManager.h"
#include "RenderPassResourceManager.h"
#include "scene/Scene.h"

using namespace coral;

RenderPassManager::RenderPassManager()
{
    connect<&RenderPassManager::invalidate>(renderPasses.itemAdded, this);
    connect<&RenderPassManager::invalidate>(renderPasses.itemRemoved, this);
}

Object<RenderPass> RenderPassManager::getRenderPassByName(const std::string& name)
{
    for (auto& renderPass : renderPasses)
    {
        if (renderPass->name == name)
        {
            return renderPass;
        }
    }
    return nullptr;
}

void RenderPassManager::update()
{
    // bake the graph
    if (orderedRenderPasses.empty())
    {
        bake();
    }
}

void RenderPassManager::bake()
{
    // clear resources
    RenderPassFramebufferManager::clear();
    RenderPassResourceManager::clear();

    // sort render passes, todo
    for (auto renderpass : renderPasses)
    {
        orderedRenderPasses.push_back(renderpass);
        renderpass->prepare();
    }
}

void RenderPassManager::invalidate()
{
    orderedRenderPasses.clear();
}

const std::vector<Object<RenderPass>>& RenderPassManager::getOrderedRenderPasses()
{
    return orderedRenderPasses;
}
