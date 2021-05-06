#pragma once
#include "BackendRenderPass.h"
#include <memory>

namespace backend::opengl
{
    class OpenglRenderPass : public BackendRenderPass
    {
    public:
        OpenglRenderPass(const BackendRenderPassParams& params);
        void begin(const BeginRenderPassParams& params) override;
        void end() override;
    };
}
#pragma once
