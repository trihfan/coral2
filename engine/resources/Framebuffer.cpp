#include "Framebuffer.h"
#include "Engine.h"
#include "Resource.h"
#include "base/ObjectFactory.h"

using namespace coral;

Framebuffer::Framebuffer()
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

void Framebuffer::bind(backend::BackendFramebufferUsage usage)
{
    backendFramebuffer->bind(usage);
}

void Framebuffer::init()
{
    Object::init();
    std::vector<backend::BackendFramebufferResource> backendResources;
    for (const auto& resource : resources)
    {
        backendResources.push_back(backend::BackendFramebufferResource { resource.role, resource.resource->getBackendResource() });
    }

    backendFramebuffer = backend::BackendObjectFactory<backend::BackendFramebuffer>::create(backendResources);
}

void Framebuffer::release()
{
    Object::release();
    backendFramebuffer = nullptr;
}

BackbufferFramebuffer::BackbufferFramebuffer()
{
}

void BackbufferFramebuffer::bind(backend::BackendFramebufferUsage usage)
{
    backbuffer->bind(usage);
}

void BackbufferFramebuffer::init()
{
    Object::init();
    backbuffer = backend::BackendObjectFactory<backend::BackendBackbufferFramebuffer>::create();
}

void BackbufferFramebuffer::release()
{
    Framebuffer::release();
    backbuffer = nullptr;
}
