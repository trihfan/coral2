#pragma once

#include "backend/interfaces/Backend.h"
#include <GL/glew.h>

namespace coral
{
    class OpenGLBackend : public Backend
    {
    public:
        bool init() override;
        bool destroy() override;
        bool resize(int width, int height) override;
    };
}
