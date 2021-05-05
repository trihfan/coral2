#include "BackendRenderPass.h"
#include "BackendObjectFactory.h"

using namespace backend;

template <>
std::function<std::unique_ptr<BackendRenderPass>(const BackendRenderPassParams&)> creator<BackendRenderPass, BackendRenderPassParams> = nullptr;
