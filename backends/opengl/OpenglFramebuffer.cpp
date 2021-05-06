#include "OpenglFramebuffer.h"
#include "BackendResource.h"
#include "Logs.h"
#include "OpenglError.h"
#include "OpenglResource.h"
#include <memory>

using namespace backend::opengl;
using namespace coral;

OpenglFramebuffer::OpenglFramebuffer(const BackendFramebufferCreationParams& params)
    : BackendFramebuffer(params)
    , framebufferId(0)
{
    // Backbuffer special case
    if (!params.resources.empty() == 1 && params.resources[0].name == backend::BackendFramebuffer::backbufferName)
    {
        return;
    }

    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    // add resources
    int colorCount = 0;
    for (const auto& resource : params.resources)
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

        GLenum target = static_cast<OpenglResource*>(resource.resource)->getSamplesCount() == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
        glFramebufferTexture2D(GL_FRAMEBUFFER, type, target, static_cast<OpenglResource*>(resource.resource)->getId(), 0);
    }

    CHECK_OPENGL_ERROR
}

OpenglFramebuffer::~OpenglFramebuffer()
{
    if (framebufferId != 0)
    {
        glDeleteFramebuffers(1, &framebufferId);
        CHECK_OPENGL_ERROR
    }
}

void OpenglFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    CHECK_OPENGL_ERROR
}