#pragma once

#include "resources/Framebuffer.h"
#include "utils/Singleton.h"
#include <memory>
#include <vector>

namespace coral
{
    class Engine;
    struct FramebufferResource;

    // The framebuffer manager
    class RenderPassFramebufferManager
    {
        MAKE_SINGLETON(RenderPassFramebufferManager)
    public:
        static void clear();
        static Handle<Framebuffer> getFramebufferFor(const std::vector<FramebufferResource>& resources);
        static Handle<Framebuffer> getBackbuffer();

    private:
        RenderPassFramebufferManager();
        std::vector<Handle<Framebuffer>> framebuffers;
        Handle<Framebuffer> defaultFramebuffer;
    };
}
