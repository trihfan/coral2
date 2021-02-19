#pragma once

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
        static Handle<Resource> getResourceByName(const std::string& name);

        // Register named resources
        static void registerResource(Handle<Resource> resource);

    private:
        RenderPassResourceManager() = default;
        std::unordered_map<std::string, Handle<Resource>> resourceByName;
    };
}
