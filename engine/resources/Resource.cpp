#include "Resource.h"
#include "BackendObjectFactory.h"

using namespace coral;

Resource::Resource()
{
    connect<&Resource::init>(Object::init, this);
    connect<&Resource::release>(Object::release, this);
}

void Resource::bind()
{
    backendResource->bind();
}

backend::BackendResource* Resource::getBackendResource() const
{
    return backendResource.get();
}

void Resource::init()
{
    backendResource = backend::BackendObjectFactory<backend::BackendResource>::create(*params);
}

void Resource::release()
{
    backendResource = nullptr;
}
