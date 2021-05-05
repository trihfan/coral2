#include "BackendFramebuffer.h"
#include "BackendObjectFactory.h"

using namespace backend;

const std::string& BackendFramebuffer::backbufferName = "backbuffer";

template <>
std::function<std::unique_ptr<BackendFramebuffer>(const BackendFramebufferCreationParams&)> creator<BackendFramebuffer, BackendFramebufferCreationParams> = nullptr;
