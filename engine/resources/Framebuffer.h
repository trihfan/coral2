#pragma once

#include <gl/glew.h>

#include "Object.h"
#include <vector>

namespace coral
{
    class Engine;
    class Resource;

    enum class ResourceRole
    {
        color,
        depth,
        stencil
    };
    struct FramebufferResource
    {
        Handle<Resource> resource;
        ResourceRole role;
    };

    //
    class Framebuffer : public Object
    {
    public:
        Framebuffer();

        /// Custom framebuffer
        Framebuffer(GLuint id);

        //
        void addResource(const FramebufferResource& resource);
        const std::vector<FramebufferResource>& getResources() const;

        void bind(GLenum target = GL_DRAW_FRAMEBUFFER);

    private:
        // initialization
        void init();
        void release();

    private:
        GLuint framebufferId;
        std::vector<FramebufferResource> resources;
    };
}
