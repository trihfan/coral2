#include "BackendResource.h"
#include "BackendObjectFactory.h"

using namespace backend;

template <>
std::function<std::unique_ptr<BackendResource>(const BackendResourceParams&)> creator<BackendResource, BackendResourceParams> = nullptr;
