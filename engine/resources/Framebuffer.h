#pragma once

#include "BackendFramebuffer.h"
#include "Object.h"
#include "Resource.h"
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

    private:
        // initialization
        virtual void init();
        virtual void release();

        std::unique_ptr<backend::BackendFramebuffer> backendFramebuffer;
        std::vector<FramebufferResource> resources;
    };

    class DefaultFramebuffer : public Object
    {
    public:
        DefaultFramebuffer();
        void bind(backend::BackendFramebufferUsage usage);

    protected:
        std::unique_ptr<backend::BackendDefaultFramebuffer> defaultFramebuffer;

        // initialization
        virtual void init();
        virtual void release();
    };
}
