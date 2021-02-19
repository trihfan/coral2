#include "OpenGLBackend.h"
#include "utils/Logs.h"

using namespace coral;

bool OpenGLBackend::init()
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

bool OpenGLBackend::destroy()
{
    return true;
}

bool OpenGLBackend::resize(int width, int height)
{
    return true;
}
