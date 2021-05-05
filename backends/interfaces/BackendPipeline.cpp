#include "BackendPipeline.h"
#include "BackendObjectFactory.h"

using namespace backend;

template <>
std::function<std::unique_ptr<BackendPipeline>(const BackendPipelineParams&)> creator<BackendPipeline, BackendPipelineParams> = nullptr;
