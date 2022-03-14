#pragma once

#include "BackendFramebuffer.h"
#include "Resource.h"
#include "base/Object.h"
#include "Handle.h"
#include <memory>
#include <vector>

namespace coral
{
    class RenderPass;

    struct FramebufferResource
    {
        backend::BackendFramebufferResourceRole role;
        Handle<Resource> resource;
    };

    class Framebuffer : public Object
    {
    public:
        // Constructor with the renderpass to make it compatible with
        Framebuffer(const Handle<RenderPass>& renderpass);

        // Framebuffer resources
        void addResource(const FramebufferResource& resource);
        const std::vector<FramebufferResource>& getResources() const;

        // Initialize
        virtual void init() override;
        virtual void release() override;

        backend::BackendFramebuffer* getBackendFramebuffer() const;

    private:
        Handle<RenderPass> renderpass;
        std::unique_Handle<backend::BackendFramebuffer> backendFramebuffer;
        std::vector<FramebufferResource> resources;
    };
}
