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
    class Scene;
    class Camera;
    class Node;

    struct RenderParameters
	{
        std::shared_ptr<Camera> camera;
        std::vector<std::shared_ptr<Node>> lights;
	};

    class Engine
    {
        MAKE_SINGLETON(Engine)
    public:
        // tmp
        static RenderParameters current_parameters;

        // creation
        static void create();
        static void destroy();

        // configuration
        static void setMemoryResource(std::unique_ptr<std::pmr::memory_resource> memory_resource);

        // run
        static void setCurrentScene(std::shared_ptr<Scene> scene);
        static void frame();

    private:
        // Constructor
        Engine();

    private:
        // Engine memory resource
        static std::unique_ptr<std::pmr::memory_resource> memory_resource;
    };
}
#endif
