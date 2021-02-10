#include "Framebuffer.h"
#include "Resource.h"
#include "utils/Error.h"

using namespace coral;

//*********************************************************************************
// Framebuffer

Framebuffer::Framebuffer() : framebufferId(0)
{
    connect<&Framebuffer::init>(Object::init, this);
    connect<&Framebuffer::release>(Object::release, this);
}

void Framebuffer::addResouce(const std::string& resource, ResourceRole type)
{
    resources.push_back(std::make_pair(resource, type));
}

const std::vector<std::pair<std::string, ResourceRole>>& Framebuffer::getResouces() const
{
    return resources;
}

void Framebuffer::bind(GLenum target)
{
    //glBindFramebuffer(target, framebufferId);
    CHECK_OPENGL_ERROR
}

void Framebuffer::init()
{
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    // add resources
    int colorCount = 0;

    for (const auto& pair : resources)
    {
        auto resource = ResourceManager::getResourceByName(pair.first);
        resource->bind();
        
        GLenum type;
        switch (pair.second)
        {
        case ResourceRole::color:
            type = GL_COLOR_ATTACHMENT0 + colorCount++;
            break;

        case ResourceRole::depth:
            type = GL_DEPTH_ATTACHMENT;
            break;

        case ResourceRole::stencil:
            type = GL_STENCIL_ATTACHMENT;
            break;
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, type, *resource->sampleCount == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, resource->getId(), 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); //tmp
    CHECK_OPENGL_ERROR
}

void Framebuffer::release()
{
    glDeleteFramebuffers(1, &framebufferId);
    CHECK_OPENGL_ERROR
}

//*********************************************************************************
// FramebufferManager

bool operator==(const std::vector<std::string>& a, const std::vector<std::pair<std::string, ResourceRole>>& b)
{
    if (a.size() == b.size())
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            if (a[i] != b[i].first)
            {
                return false;
            }
        }
    }
    return true;
}

DEFINE_SINGLETON(FramebufferManager)

FramebufferManager::FramebufferManager(std::shared_ptr<std::pmr::memory_resource> memory_resource)
{
}

void FramebufferManager::clear()
{
    instance->framebuffers.clear();
}

std::shared_ptr<Framebuffer> FramebufferManager::getFramebufferFor(const std::vector<std::pair<std::string, ResourceRole>>& resources)
{
    for (auto framebuffer : instance->framebuffers)
    {
        if (framebuffer->getResouces() == resources)
        {
            return framebuffer;
        }
    }

    std::shared_ptr<Framebuffer> framebuffer = ObjectManager::create<Framebuffer>();
    for (const auto& resource : resources)
    {
        framebuffer->addResouce(resource.first, resource.second);
    }
    instance->framebuffers.push_back(framebuffer);
    return framebuffer;
}