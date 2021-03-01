#include "Backend.h"
#include "BackendCommandBuffer.h"
#include "BackendFramebuffer.h"
#include "BackendObjectFactory.h"
#include "BackendPipeline.h"
#include "BackendResource.h"

using namespace backend;

Backend* Backend::currentBackend = nullptr;

template <>
std::function<std::unique_ptr<BackendFramebuffer>(const std::vector<BackendFramebufferResource>&)> creator<BackendFramebuffer, std::vector<BackendFramebufferResource>> = nullptr;

template <>
std::function<std::unique_ptr<BackendDefaultFramebuffer>()> creator<BackendDefaultFramebuffer> = nullptr;

template <>
std::function<std::unique_ptr<BackendPipeline>(const BackendPipelineParams&)> creator<BackendPipeline, BackendPipelineParams> = nullptr;

template <>
std::function<std::unique_ptr<BackendResource>(const BackendResourceParams&)> creator<BackendResource, BackendResourceParams> = nullptr;

template <>
std::function<std::unique_ptr<BackendCommandBuffer>()> creator<BackendCommandBuffer> = nullptr;

Backend* Backend::current()
{
    return currentBackend;
}

void Backend::setCurrent(Backend* backend)
{
    currentBackend = backend;
}
