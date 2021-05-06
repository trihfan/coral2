#pragma once

#include "BackendFramebuffer.h"
#include "Resource.h"
#include "base/Object.h"
#include "base/Ptr.h"
#include <memory>
#include <vector>

namespace coral
{
    class RenderPass;

    struct FramebufferResource
    {
        backend::BackendFramebufferResourceRole role;
        ptr<Resource> resource;
    };

    class Framebuffer : public Object
    {
    public:
        // Constructor with the renderpass to make it compatible with
        Framebuffer(const ptr<RenderPass>& renderpass);

        // Framebuffer resources
        void addResource(const FramebufferResource& resource);
        const std::vector<FramebufferResource>& getResources() const;

        // Initialize
        virtual void init() override;
        virtual void release() override;

        backend::BackendFramebuffer* getBackendFramebuffer() const;

    private:
        ptr<RenderPass> renderpass;
        std::unique_ptr<backend::BackendFramebuffer> backendFramebuffer;
        std::vector<FramebufferResource> resources;
    };
}
