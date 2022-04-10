#pragma once
#include "Object.h"
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
        Object<Framebuffer> getFramebufferFor(const Object<RenderPass>& renderpass, const std::vector<FramebufferResource>& resources);

    private:
        RenderPassFramebufferManager();
        std::vector<Object<Framebuffer>> framebuffers;
        Object<Framebuffer> defaultFramebuffer;
    };
}
