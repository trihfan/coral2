#pragma once

namespace backend
{
    class BackendCommandBuffer
    {
    public:
        virtual void begin() = 0;
        virtual void end() = 0;

        virtual void clearColor(float red, float green, float blue, float alpha) = 0;
        virtual void clearDepth() = 0;
    };
}
