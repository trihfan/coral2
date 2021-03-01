#include "BackendResource.h"

using namespace backend;

BackendResource::BackendResource(const BackendResourceParams& params)
    : params(params)
{
}

const BackendResourceParams& BackendResource::getParams() const
{
    return params;
}
