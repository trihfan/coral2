#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#ifdef __APPLE__
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#else
    #include <memory_resource>
#endif

namespace coral
{
    // transparency type
    enum Transparency { none, simple, depthPeeling, linkedList };

    // config to use for the engine
    // simplify the engine pipeline creation
    struct EngineConfig
    {
        // constructor with default config
        EngineConfig();

        // parameters
        std::shared_ptr<std::pmr::memory_resource> memoryResource;
        Transparency transparency;
    };
}
#endif
