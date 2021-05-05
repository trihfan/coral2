#pragma once
#include <string>
#include <vector>

namespace backend
{
    class BackendResource;
    class BackendRenderPass;

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
        std::string name;
        BackendFramebufferResourceRole role;
        BackendResource* resource;
    };

    struct BackendFramebufferCreationParams
    {
        BackendRenderPass* renderPass;
        std::vector<BackendFramebufferResource> resources;
    };

    class BackendFramebuffer
    {
    public:
        static const std::string& backbufferName;

        BackendFramebuffer(const BackendFramebufferCreationParams& params) { }
        virtual ~BackendFramebuffer() = default;
        virtual void bind(BackendFramebufferUsage usage) = 0;
    };
}
