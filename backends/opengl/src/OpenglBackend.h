#pragma once

#include "Backend.h"

namespace backend::opengl
{
    class OpenglBackend : public backend::Backend
    {
    public:
        bool init() override;
        bool destroy() override;
        bool resize(int width, int height) override;
        BackendCapabilities capabilities() const override;
    };
}
