#include "BackendFramebuffer.h"
#include "BackendObjectFactory.h"

using namespace backend;

template <>
std::function<std::unique_ptr<BackendFramebuffer>(const std::vector<BackendFramebufferResource>&)> creator<BackendFramebuffer, std::vector<BackendFramebufferResource>> = nullptr;

template <>
std::function<std::unique_ptr<BackendBackbufferFramebuffer>()> creator<BackendBackbufferFramebuffer> = nullptr;

BackendFramebuffer::BackendFramebuffer(const std::vector<BackendFramebufferResource>& resources)
{
}
