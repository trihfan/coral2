#pragma once

#include "Backend.h"
#include <GL/glew.h>

namespace backend::opengl
{
    class OpenglBackend : public backend::Backend
    {
    public:
        bool init() override;
        bool destroy() override;
        bool resize(int width, int height) override;

        std::unique_ptr<BackendPipeline> createPipeline(const BackendPipelineParams& params) const override;
    };
}
