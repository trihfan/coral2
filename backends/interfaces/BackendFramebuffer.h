#pragma once
#include <string>
#include <vector>

namespace backend
{
    class BackendResource;
    class BackendRenderPass;

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
    };
}
