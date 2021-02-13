#include "Resource.h"
#include "utils/Error.h"

using namespace coral;

Resource::Resource()
    : id(0)
{
    connect<&Resource::init>(Object::init, this);
    connect<&Resource::release>(Object::release, this);
}

void Resource::bind()
{
    glBindTexture(*sampleCount == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, id);
}

GLuint Resource::getId() const
{
    return id;
}

void Resource::init()
{
    glGenTextures(1, &id);
    glBindTexture(*sampleCount == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, id);
    glTexImage2D(*sampleCount == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, 0, *internalFormat, *width, *height, 0, *format, *type, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECK_OPENGL_ERROR
}

void Resource::release()
{
    glDeleteTextures(1, &id);
    CHECK_OPENGL_ERROR
}