#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#ifdef __APPLE__
#include <experimental/memory_resource>
namespace std
{
    namespace pmr = experimental::pmr;
}
#else
#include <memory_resource>
#endif
#include "Property.h"

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
