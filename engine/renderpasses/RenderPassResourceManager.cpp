#include "RenderPassResourceManager.h"

using namespace coral;

DEFINE_SINGLETON(RenderPassResourceManager)

void RenderPassResourceManager::clear()
{
    instance->resourceByName.clear();
}

std::shared_ptr<Resource> RenderPassResourceManager::getResourceByName(const std::string& name)
{
    auto it = instance->resourceByName.find(name);
    if (it != instance->resourceByName.end())
    {
        return it->second->toHandle<Resource>();
    }

    return nullptr;
}

RenderPassResourceManager::RenderPassResourceManager(std::shared_ptr<std::pmr::memory_resource> memory_resource)
{
}

void RenderPassResourceManager::registerResource(std::shared_ptr<Resource> resource)
{
    auto it = instance->resourceByName.find(resource->getName());
    if (it != instance->resourceByName.end())
    {
        Logs(error) << "Resource name is already used";
        return;
    }

    instance->resourceByName[resource->getName()] = resource;
}