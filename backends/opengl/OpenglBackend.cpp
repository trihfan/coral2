#include "OpenglBackend.h"
#include "Logs.h"
#include "OpenglFramebuffer.h"
#include "OpenglPipeline.h"
#include "OpenglRenderPass.h"
#include "OpenglResource.h"
#include "OpenglShader.h"
#include "OpenglVertexBuffer.h"
#include "gl.h"

using namespace coral;
using namespace backend::opengl;

bool OpenglBackend::internalInit()
{
#ifdef OPENGL_CORE
    if (!gladLoadGL())
    {
        Logs(error) << "Glad init error ";
        return false;
    }

    Logs(success) << "OpenGL " << GLVersion.major << "." << GLVersion.minor;
#endif

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Set creators
    creator<BackendFramebuffer, BackendFramebufferCreationParams> = [](const BackendFramebufferCreationParams& params) { return std::make_unique<OpenglFramebuffer>(params); };
    creator<BackendRenderPass, BackendRenderPassParams> = [](const BackendRenderPassParams& params) { return std::make_unique<OpenglRenderPass>(params); };
    creator<BackendPipeline, BackendPipelineParams> = [](const BackendPipelineParams& params) { return std::make_unique<OpenglPipeline>(params); };
    creator<BackendResource, BackendResourceParams> = [](const BackendResourceParams& params) { return std::make_unique<OpenglResource>(params); };
    creator<BackendVertexBuffer, BackendVertexBufferData> = [](const BackendVertexBufferData& data) { return std::make_unique<OpenglVertexBuffer>(data); };
    return true;
}

bool OpenglBackend::internalRelease()
{
    return true;
}

std::string OpenglBackend::getName() const
{
#ifdef OPENGL_ES
    return "gles";
#else
    return "gl";
#endif
}

bool OpenglBackend::resize(int, int)
{
    return true;
}

void OpenglBackend::beginFrame()
{
}

void OpenglBackend::endFrame()
{
}

backend::BackendCapabilities OpenglBackend::capabilities() const
{
    BackendCapabilities capabilities;
    return capabilities;
}
