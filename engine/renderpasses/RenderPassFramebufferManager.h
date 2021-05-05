#pragma once

#include "base/Ptr.h"
#include "resources/Framebuffer.h"
#include "utils/Singleton.h"
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
        MAKE_SINGLETON(RenderPassFramebufferManager)
    public:
        static void clear();
        static ptr<Framebuffer> getFramebufferFor(const ptr<RenderPass>& renderpass, const std::vector<FramebufferResource>& resources);

    private:
        RenderPassFramebufferManager();
        std::vector<ptr<Framebuffer>> framebuffers;
        ptr<Framebuffer> defaultFramebuffer;
    };
}
