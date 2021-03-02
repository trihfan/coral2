#pragma once

#include "BackendCommandBuffer.h"
#include <glad/glad.h>

namespace backend::opengl
{
    class OpenglCommandBuffer : public BackendCommandBuffer
    {
    public:
        void begin() override;
        void end() override;

        //
        void clearColor(float red, float green, float blue, float alpha) override;
        void clearDepth() override;

        // Draw
        void draw(int indexCount) override;
    };
}
