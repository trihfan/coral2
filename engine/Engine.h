#ifndef ENGINE_H
#define ENGINE_H

#ifdef __APPLE__
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#else
    #include <memory_resource>
#endif

#include "utils/Singleton.h"

namespace coral
{
    class Engine
    {
        MAKE_ENGINE_SINGLETON(Engine)
    public:
        // creation
        static void create();
        static void destroy();

        // configuration
        void setMemoryResource(std::unique_ptr<std::pmr::memory_resource> memory_resource);

    private:
        static std::unique_ptr<std::pmr::memory_resource> memory_resource;

    private:
        Engine();
    };
}
#endif
