#pragma once

namespace backend
{
    struct BackendRenderPassParams
    {
    };

    class BackendRenderPass
    {
    public:
        BackendRenderPass(const BackendRenderPassParams& params) { }
        virtual ~BackendRenderPass() = default;
    };
}
