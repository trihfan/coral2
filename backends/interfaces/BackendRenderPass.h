#pragma once

#include <glm/glm.hpp>

namespace backend
{
    class BackendFramebuffer;

    struct BeginRenderPassParams
    {
        BackendFramebuffer* framebuffer;
        glm::vec4 viewport;
        bool clearColor = false;
        bool clearDepth = false;
        glm::vec4 clearColorValue = glm::vec4(0, 0, 0, 0);
    };

    struct BackendRenderPassParams
    {
    };

    class BackendRenderPass
    {
    public:
        BackendRenderPass(const BackendRenderPassParams& params) { }
        virtual ~BackendRenderPass() = default;

        virtual void begin(const BeginRenderPassParams& params) = 0;
        virtual void end() = 0;
    };
}
