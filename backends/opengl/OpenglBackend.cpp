#include "OpenglBackend.h"
#include "Logs.h"

using namespace coral;

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
