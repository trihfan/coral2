#include "Framebuffer.h"
#include "Resource.h"
#include "backend/opengl/OpenglError.h"

using namespace coral;

Framebuffer::Framebuffer() : framebufferId(0)
{
    connect<&Framebuffer::init>(Object::init, this);
    connect<&Framebuffer::release>(Object::release, this);
}

Framebuffer::Framebuffer(GLuint id) : framebufferId(id)
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

void Framebuffer::bind(GLenum target)
{
    glBindFramebuffer(target, framebufferId);
    CHECK_OPENGL_ERROR
}

void Framebuffer::init()
{
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    // add resources
    int colorCount = 0;
    for (const auto& resource : resources)
    {
        resource.resource->bind();
        
        GLenum type;
        switch (resource.role)
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

        glFramebufferTexture2D(GL_FRAMEBUFFER, type, *resource.resource->sampleCount == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, resource.resource->getId(), 0);
    }

    CHECK_OPENGL_ERROR
}

void Framebuffer::release()
{
    glDeleteFramebuffers(1, &framebufferId);
    CHECK_OPENGL_ERROR
}
