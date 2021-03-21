#include "RenderPassFramebufferManager.h"
#include "base/ObjectFactory.h"
#include "resources/Framebuffer.h"
#include "resources/Resource.h"

using namespace coral;

DEFINE_SINGLETON(RenderPassFramebufferManager)

void RenderPassFramebufferManager::release()
{
}

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

void RenderPassFramebufferManager::clear()
{
    instance->framebuffers.clear();
}

Handle<Framebuffer> RenderPassFramebufferManager::getFramebufferFor(const std::vector<FramebufferResource>& resources)
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

    Handle<Framebuffer> framebuffer = ObjectFactory::create<Framebuffer>();
    for (const auto& resource : resources)
    {
        framebuffer->addResource(resource);
    }
    instance->framebuffers.push_back(framebuffer);
    return framebuffer;
}

Handle<Framebuffer> RenderPassFramebufferManager::getBackbuffer()
{
    return instance->defaultFramebuffer;
}

RenderPassFramebufferManager::RenderPassFramebufferManager()
    : defaultFramebuffer(ObjectFactory::create<DefaultFramebuffer>())
{
}
