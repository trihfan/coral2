#include "Backend.h"
#include "BackendCommandBuffer.h"
#include "BackendFramebuffer.h"
#include "BackendPipeline.h"
#include "BackendResource.h"
#include "BackendVertexBuffer.h"

using namespace backend;

Backend* Backend::currentBackend = nullptr;

Backend* Backend::current()
{
    return currentBackend;
}

void Backend::setCurrent(Backend* backend)
{
    currentBackend = backend;
}

bool Backend::init(const BackendParams& params)
{
    setCurrent(this);
    threadCount = params.threadCount;
    bool result = internalInit();
    return result;
}

bool Backend::release()
{
    return internalRelease();
}