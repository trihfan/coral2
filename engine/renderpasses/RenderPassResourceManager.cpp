#include "RenderPassResourceManager.h"
#include "Handle.h"

using namespace coral;

DEFINE_SINGLETON(RenderPassResourceManager)

void RenderPassResourceManager::release()
{
}

void RenderPassResourceManager::clear()
{
    instance->resourceByName.clear();
}

Handle<Resource> RenderPassResourceManager::getResourceByName(const std::string& name)
{
    auto it = instance->resourceByName.find(name);
    if (it != instance->resourceByName.end())
    {
        return it->second;
    }

    return nullptr;
}

void RenderPassResourceManager::registerResource(Handle<Resource> resource)
{
    auto it = instance->resourceByName.find(resource->getName());
    if (it != instance->resourceByName.end())
    {
        Logs(error) << "Resource name is already used";
        return;
    }

    instance->resourceByName[resource->getName()] = resource;
}
