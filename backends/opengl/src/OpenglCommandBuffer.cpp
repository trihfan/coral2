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
