#include "OpenglCommandBuffer.h"
#include "Logs.h"
#include "OpenglError.h"

using namespace backend::opengl;
using namespace coral;

void OpenglCommandBuffer::begin()
{
}

void OpenglCommandBuffer::end()
{
}

void OpenglCommandBuffer::setViewport(float x, float y, float width, float height)
{
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLint>(width), static_cast<GLint>(height));
}

void OpenglCommandBuffer::clearColor(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
    CHECK_OPENGL_ERROR
}

void OpenglCommandBuffer::clearDepth()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    CHECK_OPENGL_ERROR
}

void OpenglCommandBuffer::draw(int indexCount)
{
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
}
