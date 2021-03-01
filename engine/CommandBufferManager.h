#pragma once

#include "BackendCommandBuffer.h"
#include "Object.h"
#include "utils/Singleton.h"
#include <vector>

namespace coral
{
    enum class CommandBufferStage : int
    {
        update,
        draw
    };

    class CommandBufferManager
    {
        MAKE_SINGLETON(CommandBufferManager)
    public:
        static backend::BackendCommandBuffer* getCommandBuffer(int threadId = 0);
        static void submit(CommandBufferStage stage);

    private:
        std::vector<std::unique_ptr<backend::BackendCommandBuffer>> commandBuffers;
        std::vector<bool> usedCommandBuffers;

        CommandBufferManager();
    };
}
