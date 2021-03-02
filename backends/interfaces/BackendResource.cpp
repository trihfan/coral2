#include "BackendResource.h"
#include "BackendObjectFactory.h"

using namespace backend;

template <>
std::function<std::unique_ptr<BackendResource>(const BackendResourceParams&)> creator<BackendResource, BackendResourceParams> = nullptr;

BackendResource::BackendResource(const BackendResourceParams& params)
    : params(params)
{
}

const BackendResourceParams& BackendResource::getParams() const
{
    return params;
}
