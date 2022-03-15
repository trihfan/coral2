#pragma once
#include "Handle.h"
#include "resources/Framebuffer.h"
#include <memory>
#include <vector>

namespace coral
{
    class Engine;
    class RenderPass;
    struct FramebufferResource;

    // The framebuffer manager
    class RenderPassFramebufferManager
    {
    public:
        void clear();
        Handle<Framebuffer> getFramebufferFor(const Handle<RenderPass>& renderpass, const std::vector<FramebufferResource>& resources);

    private:
        RenderPassFramebufferManager();
        std::vector<Handle<Framebuffer>> framebuffers;
        Handle<Framebuffer> defaultFramebuffer;
    };
}
