#include "Framebuffer.h"
#include "utils/Error.h"

using namespace coral;

//*********************************************************************************
// Framebuffer

Framebuffer::Framebuffer() : framebufferId(0)
{
    connect<&Framebuffer::init>(Object::init, this);
    connect<&Framebuffer::release>(Object::release, this);
}

void Framebuffer::addResouce(const Resource& resouce)
{
}

void Framebuffer::setNumberOfSamples(int numberOfSamples)
{
}

void Framebuffer::bind(GLenum target)
{
    glBindFramebuffer(target, 0);//glBindFramebuffer(target, framebufferId);
    CHECK_OPENGL_ERROR
}

void Framebuffer::init()
{
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    CHECK_OPENGL_ERROR
}

void Framebuffer::release()
{
    glDeleteFramebuffers(1, &framebufferId);
    CHECK_OPENGL_ERROR
}

//*********************************************************************************
// FramebufferManager

DEFINE_SINGLETON(FramebufferManager)

FramebufferManager::FramebufferManager(std::shared_ptr<std::pmr::memory_resource> memory_resource)
{
}

void FramebufferManager::clear()
{
    instance->frambufferByName.clear();
    instance->framebuffers.clear();
}

std::shared_ptr<Framebuffer> FramebufferManager::getFramebuffer(const std::string& name)
{
    auto it = instance->frambufferByName.find(name);
    if (it != instance->frambufferByName.end())
    {
        return it->second;
    }

    std::shared_ptr<Framebuffer> framebuffer = ObjectManager::createWithName<Framebuffer>(name);
    instance->frambufferByName[name] = framebuffer;
    return framebuffer;
}

std::shared_ptr<Framebuffer> FramebufferManager::getFramebufferFor(const std::vector<std::string>& colorOutputs)
{
    for (auto framebuffer : instance->framebuffers)
    {
        return framebuffer;
    }

    return getFramebuffer("new");
}