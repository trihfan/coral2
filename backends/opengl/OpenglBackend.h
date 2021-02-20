#pragma once

#include "Backend.h"
#include <GL/glew.h>

namespace coral
{
    class OpenglBackend : public Backend
    {
    public:
        bool init() override;
        bool destroy() override;
        bool resize(int width, int height) override;
    };
}
