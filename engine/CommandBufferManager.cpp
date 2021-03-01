#include "CommandBufferManager.h"
#include "Backend.h"
#include "Engine.h"
#include "EngineConfig.h"

using namespace coral;

DEFINE_SINGLETON(CommandBufferManager)

CommandBufferManager::CommandBufferManager()
{
    commandBuffers.resize(static_cast<size_t>(backend::Backend::current()->capabilities().multithreadCapable ? EngineConfig::parameters.threadCount : 1));
    usedCommandBuffers.resize(commandBuffers.size(), false);
    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        commandBuffers[i] = backend::BackendObjectFactory<backend::BackendCommandBuffer>::create();
    }
}

void CommandBufferManager::release()
{
    instance->commandBuffers.clear();
}

backend::BackendCommandBuffer* CommandBufferManager::getCommandBuffer(int threadId)
{
    size_t index = static_cast<size_t>(threadId);
    if (!instance->usedCommandBuffers[index])
    {
        instance->commandBuffers[index]->begin();
        instance->usedCommandBuffers[index] = true;
    }
    return instance->commandBuffers[index].get();
}

void CommandBufferManager::submit(CommandBufferStage stage)
{
    // List used command buffer
    std::vector<backend::BackendCommandBuffer*> commandBuffers;
    for (size_t i = 0; i < instance->usedCommandBuffers.size(); i++)
    {
        if (instance->usedCommandBuffers[i])
        {
            instance->commandBuffers[i]->end();
            commandBuffers.push_back(instance->commandBuffers[i].get());
            instance->usedCommandBuffers[i] = false;
        }
    }

    // Submit to queue
    //BackendQueue::get(stage)->submit(commandBuffers);
}
