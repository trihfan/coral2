#include "OpenglFramebuffer.h"
#include "BackendResource.h"
#include "Logs.h"
#include "OpenglError.h"
#include "OpenglResource.h"
#include <memory>

using namespace backend::opengl;
using namespace coral;

OpenglFramebuffer::OpenglFramebuffer(const std::vector<BackendFramebufferResource>& resources)
    : BackendFramebuffer(resources)
    , framebufferId(0)
{
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    // add resources
    int colorCount = 0;
    for (const auto& resource : resources)
    {
        resource.resource->bind(0);

        GLenum type;
        switch (resource.role)
        {
        case BackendFramebufferResourceRole::color:
            type = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + colorCount++);
            break;

        case BackendFramebufferResourceRole::depth:
            type = GL_DEPTH_ATTACHMENT;
            break;

        case BackendFramebufferResourceRole::stencil:
            type = GL_STENCIL_ATTACHMENT;
            break;
        }

        GLenum target = resource.resource->getParams().samples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
        glFramebufferTexture2D(GL_FRAMEBUFFER, type, target, static_cast<OpenglResource*>(resource.resource)->getId(), 0);
    }

    CHECK_OPENGL_ERROR
}

OpenglFramebuffer::~OpenglFramebuffer()
{
    glDeleteFramebuffers(1, &framebufferId);
    CHECK_OPENGL_ERROR
}

void OpenglFramebuffer::bind(BackendFramebufferUsage usage)
{
    switch (usage)
    {
    case BackendFramebufferUsage::read:
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferId);
        break;

    case BackendFramebufferUsage::write:
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferId);
        break;

    case BackendFramebufferUsage::readAndWrite:
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
        break;
    }

    CHECK_OPENGL_ERROR
}

void OpenglBackendBackbufferFramebuffer::bind(BackendFramebufferUsage usage)
{
    switch (usage)
    {
    case BackendFramebufferUsage::read:
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        break;

    case BackendFramebufferUsage::write:
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        break;

    case BackendFramebufferUsage::readAndWrite:
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        break;
    }

    CHECK_OPENGL_ERROR
}
