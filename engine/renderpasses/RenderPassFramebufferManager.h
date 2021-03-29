#pragma once

#include "base/Ptr.h"
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
        static ptr<Framebuffer> getFramebufferFor(const std::vector<FramebufferResource>& resources);
        static ptr<Framebuffer> getBackbuffer();

    private:
        RenderPassFramebufferManager(ptr<Framebuffer> defaultFramebuffer);
        std::vector<ptr<Framebuffer>> framebuffers;
        ptr<Framebuffer> defaultFramebuffer;
    };
}
