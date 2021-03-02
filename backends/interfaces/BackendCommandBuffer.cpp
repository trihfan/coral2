#include "BackendCommandBuffer.h"
#include "Backend.h"
#include "BackendObjectFactory.h"
#include <thread>

using namespace backend;

template <>
std::function<std::unique_ptr<BackendCommandBuffer>()> creator<BackendCommandBuffer> = nullptr;

std::vector<std::unique_ptr<backend::BackendCommandBuffer>> BackendCommandBuffer::commandBuffers;
std::vector<bool> BackendCommandBuffer::usedCommandBuffers;

void BackendCommandBuffer::init(const BackendParams& params)
{
    commandBuffers.resize(static_cast<size_t>(Backend::current()->capabilities().multithreadCapable ? params.threadCount : 1));
    usedCommandBuffers.resize(commandBuffers.size(), false);
    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        commandBuffers[i] = backend::BackendObjectFactory<backend::BackendCommandBuffer>::create();
    }
}

void BackendCommandBuffer::release()
{
    commandBuffers.clear();
}

BackendCommandBuffer* BackendCommandBuffer::getCommandBuffer()
{
    int threadId = 0;
    size_t index = static_cast<size_t>(threadId);
    if (!usedCommandBuffers[index])
    {
        commandBuffers[index]->begin();
        usedCommandBuffers[index] = true;
    }
    return commandBuffers[index].get();
}

void BackendCommandBuffer::submit(BackendCommandBufferStage stage)
{
    // List used command buffer
    std::vector<backend::BackendCommandBuffer*> commandBuffersToSubmit;
    for (size_t i = 0; i < usedCommandBuffers.size(); i++)
    {
        if (usedCommandBuffers[i])
        {
            commandBuffers[i]->end();
            commandBuffersToSubmit.push_back(commandBuffers[i].get());
            usedCommandBuffers[i] = false;
        }
    }

    // Submit to queue
    //BackendQueue::get(stage)->submit(commandBuffers);
}
