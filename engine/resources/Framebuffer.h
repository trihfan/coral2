#pragma once

#include "BackendFramebuffer.h"
#include "Resource.h"
#include "base/Object.h"
#include <memory>
#include <vector>

namespace coral
{
    struct FramebufferResource
    {
        backend::BackendFramebufferResourceRole role;
        Handle<Resource> resource;
    };

    class Framebuffer : public Object
    {
    public:
        Framebuffer();

        void addResource(const FramebufferResource& resource);
        const std::vector<FramebufferResource>& getResources() const;

        void bind(backend::BackendFramebufferUsage usage);

        virtual void init() override;
        virtual void release() override;

    private:
        std::unique_ptr<backend::BackendFramebuffer> backendFramebuffer;
        std::vector<FramebufferResource> resources;
    };

    class DefaultFramebuffer : public Object
    {
    public:
        DefaultFramebuffer();
        void bind(backend::BackendFramebufferUsage usage);

        virtual void init() override;
        virtual void release() override;

    protected:
        std::unique_ptr<backend::BackendDefaultFramebuffer> defaultFramebuffer;
    };
}
