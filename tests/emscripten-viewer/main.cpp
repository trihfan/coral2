#include "Engine.h"
#include "backend/opengl/OpenglBackend.h"
#include "scene/camera/Camera.h"

#include <emscripten.h>
#include <glfw/GLFW3.h>

using namespace coral;

int width;
int height;

extern "C" int initialize(int w, int h)
{
    width = w;
    height = h;

    return 1;
}

extern "C" void update()
{
}
