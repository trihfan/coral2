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
    bool result = internalInit();
    setCurrent(this);
    BackendCommandBuffer::init(params);
    return result;
}

bool Backend::release()
{
    BackendCommandBuffer::release();
    return internalRelease();
}
