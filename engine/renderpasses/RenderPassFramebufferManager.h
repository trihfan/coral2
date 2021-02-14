#ifndef RENDERPASSFRAMEBUFFERMANAGER_H
#define RENDERPASSFRAMEBUFFERMANAGER_H

#include "resources/Framebuffer.h"
#include "memory_resource.h"
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
        static std::shared_ptr<Framebuffer> getFramebufferFor(const std::vector<FramebufferResource>& resources);

    private:
        RenderPassFramebufferManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);

    private:
        std::vector<std::shared_ptr<Framebuffer>> framebuffers;
    };
}
#endif
