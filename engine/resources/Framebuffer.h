#pragma once
#include "Object.h"
#include "Resource.h"
#include "Property.h"
#include "PropertyArray.h"
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace coral
{
    class RenderPass;

    enum class FramebufferResourceRole
    {
        color,
        depth,
        stencil
    };

    struct FramebufferResource
    {
        FramebufferResourceRole role;
        std::shared_ptr<Resource> resource;
    };

    inline static std::string backbufferName = "backbuffer";

    class Framebuffer : public Object
    {
    public:
        // Construction
        Framebuffer();

        // Properties
        Property<glm::ivec2> extend;
        Property<std::shared_ptr<RenderPass>> renderPass;
        PropertyArray<FramebufferResource> resources;

        // Initialize
        virtual void init() override;
        virtual void release() override;

    private:
        // Vulkan data
        VkDevice device;
        VkFramebuffer framebuffer;

        // If true this framebuffer is a ref to the backbuffer
        bool linkWithBackbuffer;
    };
}
