#include "OpenglBackend.h"
#include "Logs.h"
#include "OpenglPipeline.h"

using namespace coral;
using namespace backend::opengl;

bool OpenglBackend::init()
{
#ifndef __EMSCRIPTEN__
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        Logs(error) << "Glew init error " << glewGetErrorString(err);
        return false;
    }
    Logs(success) << "Using glew " << glewGetString(GLEW_VERSION);
#endif
    return true;
}

bool OpenglBackend::destroy()
{
    return true;
}

bool OpenglBackend::resize(int width, int height)
{
    return true;
}

std::unique_ptr<backend::BackendPipeline> OpenglBackend::createPipeline(const BackendPipelineParams& params) const
{
    return std::make_unique<OpenglPipeline>(params);
}
