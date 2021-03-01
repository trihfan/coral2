#pragma once
#include "BackendObjectFactory.h"
#include <memory>

namespace backend
{
    class BackendResource;

    enum class BackendFramebufferUsage
    {
        read,
        write,
        readAndWrite
    };

    enum class BackendFramebufferResourceRole
    {
        color,
        depth,
        stencil
    };

    struct BackendFramebufferResource
    {
        BackendFramebufferResourceRole role;
        BackendResource* resource;
    };

    class BackendFramebuffer
    {
    public:
        BackendFramebuffer(const std::vector<BackendFramebufferResource>& resources);
        virtual ~BackendFramebuffer() = default;
        virtual void bind(BackendFramebufferUsage usage) = 0;
    };

    class BackendDefaultFramebuffer
    {
    public:
        virtual ~BackendDefaultFramebuffer() = default;
        virtual void bind(BackendFramebufferUsage usage) = 0;
    };
}
