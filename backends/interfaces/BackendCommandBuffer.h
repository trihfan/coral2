#pragma once
#include <vector>

namespace backend
{
    struct BackendParams;

    enum class BackendCommandBufferStage : int
    {
        staging,
        draw
    };

    class BackendCommandBuffer
    {
    public:
        // Construction
        BackendCommandBuffer() = default;
        virtual ~BackendCommandBuffer() = default;

        // Use
        virtual void begin() = 0;
        virtual void end() = 0;

        //
        virtual void clearColor(float red, float green, float blue, float alpha) = 0;
        virtual void clearDepth() = 0;
        virtual void setViewport(float x, float y, float width, float height) = 0;

        // Draw
        virtual void draw(int indexCount) = 0;
    };

    class BackendCommandBufferManager
    {
    public:
        static void setInstance(std::unique_ptr<BackendCommandBufferManager> newInstance);

        // Return the command buffer for the current thread
        static BackendCommandBuffer* getCommandBuffer();

        // Submit the command buffers to the given stage
        static void submit(BackendCommandBufferStage stage);

    public:
        virtual ~BackendCommandBufferManager() = default;

        virtual BackendCommandBuffer* internalGetCommandBuffer() = 0;
        virtual void internalSubmit(BackendCommandBufferStage stage) = 0;

    protected:
        static std::unique_ptr<BackendCommandBufferManager> instance;
    };
}
