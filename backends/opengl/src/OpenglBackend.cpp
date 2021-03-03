#include "OpenglBackend.h"
#include "Logs.h"
#include "OpenglCommandBuffer.h"
#include "OpenglFramebuffer.h"
#include "OpenglPipeline.h"
#include "OpenglResource.h"
#include "OpenglShader.h"
#include "OpenglVertexBuffer.h"
#include "gl.h"

using namespace coral;
using namespace backend::opengl;

bool OpenglBackend::internalInit()
{
#ifndef __EMSCRIPTEN__
    if (!gladLoadGL())
    {
        Logs(error) << "Glad init error ";
        return false;
    }

    Logs(success) << "OpenGL " << GLVersion.major << "." << GLVersion.minor;
#endif

    // Set creators
    creator<BackendFramebuffer, std::vector<BackendFramebufferResource>> = [](const std::vector<BackendFramebufferResource>& resources) { return std::make_unique<OpenglFramebuffer>(resources); };
    creator<BackendDefaultFramebuffer> = []() { return std::make_unique<OpenglDefaultFramebuffer>(); };
    creator<BackendPipeline, BackendPipelineParams> = [](const BackendPipelineParams& params) { return std::make_unique<OpenglPipeline>(params); };
    creator<BackendResource, BackendResourceParams> = [](const BackendResourceParams& params) { return std::make_unique<OpenglResource>(params); };
    creator<BackendCommandBuffer> = []() { return std::make_unique<OpenglCommandBuffer>(); };
    creator<BackendVertexBuffer, BackendVertexBufferParams, BackendVertexBufferData> = [](const BackendVertexBufferParams& params, const BackendVertexBufferData& data) { return std::make_unique<OpenglVertexBuffer>(params, data); };

    return true;
}

bool OpenglBackend::internalRelease()
{
    return true;
}

bool OpenglBackend::resize(int, int)
{
    return true;
}

backend::BackendCapabilities OpenglBackend::capabilities() const
{
    BackendCapabilities capabilities;
    return capabilities;
}
