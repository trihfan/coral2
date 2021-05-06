#include "OpenglRenderPass.h"
#include "OpenglError.h"
#include "OpenglFramebuffer.h"
#include "gl.h"

using namespace backend::opengl;

OpenglRenderPass::OpenglRenderPass(const BackendRenderPassParams& params)
    : BackendRenderPass(params)
{
}

void OpenglRenderPass::begin(const BeginRenderPassParams& params)
{
    static_cast<OpenglFramebuffer*>(params.framebuffer)->bind();

    glViewport(static_cast<GLint>(params.viewport[0]), static_cast<GLint>(params.viewport[1]),
        static_cast<GLint>(params.viewport[2]), static_cast<GLint>(params.viewport[3]));

    if (params.clearColor)
    {
        glClearColor(params.clearColorValue[0], params.clearColorValue[0], params.clearColorValue[2], params.clearColorValue[3]);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    if (params.clearDepth)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    CHECK_OPENGL_ERROR
}

void OpenglRenderPass::end()
{
}