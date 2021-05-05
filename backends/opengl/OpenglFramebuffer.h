#pragma once

#include "BackendFramebuffer.h"
#include "gl.h"
#include <vector>

namespace backend::opengl
{
    class OpenglFramebuffer : public BackendFramebuffer
    {
    public:
        OpenglFramebuffer(const BackendFramebufferCreationParams& params);
        ~OpenglFramebuffer() override;
        void bind(BackendFramebufferUsage usage) override;

    private:
        GLuint framebufferId;
    };
}
