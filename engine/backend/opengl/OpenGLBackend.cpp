#include "OpenGLBackend.h"
#include "utils/Logs.h"
#include <glad/glad.h>

using namespace coral;

bool OpenGLBackend::init()
{
    if (!gladLoadGL())
    {
        Logs(error) << "Failed to initialize GLAD";
        return false;
    }

    return true;
}

bool OpenGLBackend::destroy()
{
    return true;
}