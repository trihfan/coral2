#ifndef RENDERPASSRESOURCEMANAGER_H
#define RENDERPASSRESOURCEMANAGER_H

#include "resources/Resource.h"
#include "utils/Singleton.h"
#include <memory>
#include <unordered_map>

namespace coral
{
    class Engine;

    // The resource manager
    class RenderPassResourceManager
    {
        MAKE_SINGLETON(RenderPassResourceManager)

    public:
        static void clear();
        static std::shared_ptr<Resource> getResourceByName(const std::string& name);

        // Register named resources
        static void registerResource(std::shared_ptr<Resource> resource);

    private:
        RenderPassResourceManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);

    private:
        std::unordered_map<std::string, std::shared_ptr<Resource>> resourceByName;
    };
}
#endif
