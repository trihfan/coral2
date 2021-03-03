#pragma once

#include "BackendFramebuffer.h"
#include "gl.h"
#include <vector>

namespace backend::opengl
{
    class OpenglFramebuffer : public BackendFramebuffer
    {
    public:
        OpenglFramebuffer(const std::vector<BackendFramebufferResource>& resources);
        ~OpenglFramebuffer() override;
        void bind(BackendFramebufferUsage usage) override;

    private:
        GLuint framebufferId;
    };

    class OpenglDefaultFramebuffer : public BackendDefaultFramebuffer
    {
    public:
        void bind(BackendFramebufferUsage usage) override;
    };
}
