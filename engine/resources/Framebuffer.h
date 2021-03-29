#pragma once

#include "BackendFramebuffer.h"
#include "Resource.h"
#include "base/Object.h"
#include "base/Ptr.h"
#include <memory>
#include <vector>

namespace coral
{
    struct FramebufferResource
    {
        backend::BackendFramebufferResourceRole role;
        ptr<Resource> resource;
    };

    class Framebuffer : public Object
    {
    public:
        Framebuffer();

        void addResource(const FramebufferResource& resource);
        const std::vector<FramebufferResource>& getResources() const;

        virtual void bind(backend::BackendFramebufferUsage usage);

        virtual void init() override;
        virtual void release() override;

    private:
        std::unique_ptr<backend::BackendFramebuffer> backendFramebuffer;
        std::vector<FramebufferResource> resources;
    };

    /**
     * Special framebuffer for binding to the backbuffer
     */
    class BackbufferFramebuffer : public Framebuffer
    {
    public:
        BackbufferFramebuffer();
        void bind(backend::BackendFramebufferUsage usage) override;

        virtual void init() override;
        virtual void release() override;

    protected:
        std::unique_ptr<backend::BackendBackbufferFramebuffer> backbuffer;
    };
}
