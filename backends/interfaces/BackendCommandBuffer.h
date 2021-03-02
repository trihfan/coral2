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
        BackendCommandBuffer() = default;
        virtual ~BackendCommandBuffer() = default;

        // Manage
        // Return the command buffer for the current thread
        static BackendCommandBuffer* getCommandBuffer();

        // Submit the command buffers to the given stage
        static void submit(BackendCommandBufferStage stage);

        // Use
        virtual void begin() = 0;
        virtual void end() = 0;

        //
        virtual void clearColor(float red, float green, float blue, float alpha) = 0;
        virtual void clearDepth() = 0;

        // Draw
        virtual void draw(int indexCount) = 0;

    private:
        friend class Backend;
        static std::vector<std::unique_ptr<backend::BackendCommandBuffer>> commandBuffers;
        static std::vector<bool> usedCommandBuffers;

        static void init(const BackendParams& params);
        static void release();
    };
}
