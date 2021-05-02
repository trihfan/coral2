#pragma once
#include "BackendRenderPass.h"
#include <memory>

namespace backend::opengl
{
    class OpenglRenderPass : public BackendRenderPass
    {
    public:
        OpenglRenderPass(const BackendRenderPassParams& params);
    };
}
#pragma once
