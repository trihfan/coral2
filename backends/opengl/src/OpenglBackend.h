#pragma once

#include "Backend.h"

namespace backend::opengl
{
    class OpenglBackend : public backend::Backend
    {
    public:
        bool resize(int width, int height) override;
        BackendCapabilities capabilities() const override;

    private:
        bool internalInit() override;
        bool internalRelease() override;
    };
}
