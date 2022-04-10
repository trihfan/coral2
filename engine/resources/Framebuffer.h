#pragma once
#include "Object.h"
#include "ObjectInterface.h"
#include "Resource.h"
#include "Property.h"
#include "PropertyArray.h"
#include "vulkan/VulkanBackendStructures.h"
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
        Object<Resource> resource;
    };

    inline static std::string backbufferName = "backbuffer";

    class Framebuffer : public ObjectInterface
    {
    public:
        // Construction
        Framebuffer();

        // Properties
        Property<glm::ivec2> extend;
        Property<Object<RenderPass>> renderPass;
        PropertyArray<FramebufferResource> resources;

        // Initialize
        virtual void init() override;
        virtual void release() override;

    private:
        // Vulkan data
        VulkanDevice device;
        VkFramebuffer framebuffer;

        // If true this framebuffer is a ref to the backbuffer
        bool linkWithBackbuffer;
    };
}
