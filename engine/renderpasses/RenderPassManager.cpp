#include "RenderPassManager.h"
#include "RenderPass.h"
#include "resources/Framebuffer.h"
#include "resources/Resource.h"
#include "scene/Scene.h"

using namespace coral;

DEFINE_SINGLETON(RenderPassManager)

void RenderPassManager::addRenderPass(std::shared_ptr<RenderPass> renderPass)
{
    instance->renderPasses.push_back(renderPass);
    instance->orderedRenderPasses.clear();
}

void RenderPassManager::removeRenderPass(const std::string& name)
{
    auto it = std::find_if(instance->renderPasses.begin(), instance->renderPasses.end(), [name](const std::shared_ptr<RenderPass> renderpass){ return renderpass->getName() == name; });
    if (it != instance->renderPasses.end())
    {
        instance->renderPasses.erase(it);
        instance->orderedRenderPasses.clear();
    }
}

std::shared_ptr<RenderPass> RenderPassManager::getRenderPass(const std::string& name)
{
    auto it = std::find_if(instance->renderPasses.begin(), instance->renderPasses.end(), [name](const std::shared_ptr<RenderPass> renderpass){ return renderpass->getName() == name; });
    if (it != instance->renderPasses.end())
    {
        return *it;
    }
    return nullptr;
}

void RenderPassManager::update()
{
    // bake the graph
    if (instance->orderedRenderPasses.empty())
    {
        instance->bake();
    }
}

RenderPassManager::RenderPassManager(std::shared_ptr<std::pmr::memory_resource> memory_resource)
{
}

void RenderPassManager::bake()
{
    // clear resources
    FramebufferManager::clear();
    ResourceManager::clear();

    // sort render passes
    for (auto& renderpass : renderPasses)
    {
        orderedRenderPasses.push_back(renderpass);
        renderpass->prepare();
    }
}