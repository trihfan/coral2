#include "Backend.h"
#include "BackendFramebuffer.h"
#include "BackendPipeline.h"
#include "BackendResource.h"
#include "BackendVertexBuffer.h"

using namespace backend;

bool Backend::init(const BackendParams& params)
{
    threadCount = params.threadCount;
    bool result = internalInit();
    return result;
}

bool Backend::release()
{
    return internalRelease();
}