#pragma once
#include "Handle.h"
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
        Handle<Resource> getResourceByName(const std::string& name);

        // Register named resources
        void registerResource(Handle<Resource> resource);

    private:
        std::unordered_map<std::string, Handle<Resource>> resourceByName;
    };
}
