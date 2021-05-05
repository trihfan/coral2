#include "BackendCommandBuffer.h"
#include "Backend.h"
#include "BackendObjectFactory.h"
#include <thread>

using namespace backend;

std::unique_ptr<BackendCommandBufferManager> BackendCommandBufferManager::instance;

void BackendCommandBufferManager::setInstance(std::unique_ptr<BackendCommandBufferManager> newInstance)
{
    instance = std::move(newInstance);
}

BackendCommandBuffer* BackendCommandBufferManager::getCommandBuffer()
{
    return instance->internalGetCommandBuffer();
}

void BackendCommandBufferManager::submit(BackendCommandBufferStage stage)
{
    instance->internalSubmit(stage);
}