#include "Framebuffer.h"

using namespace coral;

DEFINE_SINGLETON(FramebufferManager)

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
    glBindFramebuffer(target, framebufferId);
}

void init()
{
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
}

void release()
{
    glDeleteFramebuffers(1, &framebufferId);
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
    return nullptr;
}