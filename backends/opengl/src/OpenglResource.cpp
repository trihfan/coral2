#include "OpenglResource.h"
#include "Logs.h"
#include "OpenglError.h"

using namespace backend::opengl;
using namespace coral;

OpenglResource::OpenglResource(const backend::BackendResourceParams& params)
    : BackendResource(params)
    , id(0)
{
    glGenTextures(1, &id);
    glBindTexture(params.samples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, id);
    glTexImage2D(params.samples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, 0, getInternalFormatFrom(params.format), params.width, params.height, 0, getFormatFrom(params.format), getTypeFrom(params.format), nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECK_OPENGL_ERROR
}

OpenglResource::~OpenglResource()
{
    glDeleteTextures(1, &id);
    CHECK_OPENGL_ERROR
}

void OpenglResource::bind()
{
    glActiveTexture(GL_TEXTURE0); // tmp texture id is always 0
    glBindTexture(params.samples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, id);
}

GLuint OpenglResource::getId() const
{
    return id;
}

GLint OpenglResource::getInternalFormatFrom(backend::BackendResourceFormat format)
{
    switch (format)
    {
    case backend::BackendResourceFormat::r8g8b8a8u:
        return GL_RGBA;

    case backend::BackendResourceFormat::depth24_pencil8:
        return GL_DEPTH_STENCIL;

    default:
        Logs(error) << "Unimplemented format";
        return 0;
    }
}

GLenum OpenglResource::getFormatFrom(backend::BackendResourceFormat format)
{
    switch (format)
    {
    case backend::BackendResourceFormat::r8g8b8a8u:
        return GL_RGBA;

    case backend::BackendResourceFormat::depth24_pencil8:
        return GL_DEPTH_STENCIL;

    default:
        Logs(error) << "Unimplemented format";
        return 0;
    }
}
GLenum OpenglResource::getTypeFrom(backend::BackendResourceFormat format)
{
    switch (format)
    {
    case backend::BackendResourceFormat::r8g8b8a8u:
        return GL_UNSIGNED_INT;

    case backend::BackendResourceFormat::depth24_pencil8:
        return GL_UNSIGNED_INT_24_8;

    default:
        Logs(error) << "Unimplemented format";
        return 0;
    }
}