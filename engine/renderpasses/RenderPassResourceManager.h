#pragma once

#include "Singleton.h"
#include "base/Ptr.h"
#include "resources/Resource.h"
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
        static ptr<Resource> getResourceByName(const std::string& name);

        // Register named resources
        static void registerResource(ptr<Resource> resource);

    private:
        RenderPassResourceManager() = default;
        std::unordered_map<std::string, ptr<Resource>> resourceByName;
    };
}
