#pragma once
#include "Object.h"
#include "resources/Resource.h"
#include <memory>
#include <unordered_map>

namespace coral
{
    class Engine;

    // The resource manager
    class RenderPassResourceManager
    {
    public:
        void clear();
        Object<Resource> getResourceByName(const std::string& name);

        // Register named resources
        void registerResource(Object<Resource> resource);

    private:
        std::unordered_map<std::string, Object<Resource>> resourceByName;
    };
}
