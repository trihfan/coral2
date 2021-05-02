#pragma once

#include "BackendCommandBuffer.h"

namespace backend::vulkan
{
    class VulkanCommandBuffer : public BackendCommandBuffer
    {
    public:
        void begin() override;
        void end() override;

        //
        void setViewport(float x, float y, float width, float height) override;
        void clearColor(float red, float green, float blue, float alpha) override;
        void clearDepth() override;

        // Draw
        void draw(int indexCount) override;
    };
}
