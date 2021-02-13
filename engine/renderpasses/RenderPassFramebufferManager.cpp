#include "RenderPassFramebufferManager.h"

using namespace coral;

DEFINE_SINGLETON(RenderPassFramebufferManager)

bool operator==(std::vector<FramebufferResource> a, const std::vector<FramebufferResource>& b)
{
    if (a.size() == b.size())
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            if (a[i].resource->getName() != b[i].resource->getName() || a[i].role != b[i].role)
            {
                return false;
            }
        }
    }
    return true;
}

RenderPassFramebufferManager::RenderPassFramebufferManager(std::shared_ptr<std::pmr::memory_resource> memory_resource)
{
}

void RenderPassFramebufferManager::clear()
{
    instance->framebuffers.clear();
}

std::shared_ptr<Framebuffer> RenderPassFramebufferManager::getFramebufferFor(const std::vector<FramebufferResource>& resources)
{
    if (resources.empty())
    {
        return nullptr;
    }

    for (auto framebuffer : instance->framebuffers)
    {
        if (framebuffer->getResources() == resources)
        {
            return framebuffer;
        }
    }

    std::shared_ptr<Framebuffer> framebuffer = ObjectManager::create<Framebuffer>();
    for (const auto& resource : resources)
    {
        framebuffer->addResource(resource);
    }
    instance->framebuffers.push_back(framebuffer);
    return framebuffer;
}