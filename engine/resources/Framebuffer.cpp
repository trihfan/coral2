#include "Framebuffer.h"
#include "Engine.h"
#include "Resource.h"
#include "base/ObjectFactory.h"
#include "renderpasses/RenderPass.h"

using namespace coral;

Framebuffer::Framebuffer(const Handle<RenderPass>& renderpass)
    : renderpass(renderpass)
{
}

void Framebuffer::addResource(const FramebufferResource& resource)
{
    resources.push_back(resource);
}

const std::vector<FramebufferResource>& Framebuffer::getResources() const
{
    return resources;
}

backend::BackendFramebuffer* Framebuffer::getBackendFramebuffer() const
{
    return backendFramebuffer.get();
}

void Framebuffer::init()
{
    Object::init();

    backend::BackendFramebufferCreationParams params;
    params.renderPass = renderpass->getBackendRenderPass();

    for (const auto& resource : resources)
    {
        params.resources.push_back(backend::BackendFramebufferResource { resource.resource->getName(), resource.role, resource.resource->getBackendResource() });
    }

    backendFramebuffer = backend::BackendObjectFactory<backend::BackendFramebuffer>::create(params);
}

void Framebuffer::release()
{
    Object::release();
    backendFramebuffer = nullptr;
}