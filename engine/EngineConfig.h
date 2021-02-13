#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include "Property.h"
#include "memory_resource.h"

namespace coral
{
    // config to use for the engine
    // simplify the engine pipeline creation
    class EngineConfig
    {
    public:
        EngineConfig();
        void setup() const;

        Property<std::shared_ptr<std::pmr::memory_resource>> memoryResource;
    };
}
#endif
