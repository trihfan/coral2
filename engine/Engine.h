#ifndef ENGINE_H
#define ENGINE_H

#ifdef WIN32
    #include <memory_resource>
#else
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#endif

#include "utils/Singleton.h"
#include <vector>
#include <memory>
#include <chrono>

namespace coral
{
    class Scene;
    class Camera;
    class Node;

    struct RenderParameters
	{
        std::shared_ptr<Camera> camera;
        std::vector<std::shared_ptr<Node>> lights;
        double time;
        double deltaTime;
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

        // todo -> RenderQueueManager?
        void cull();
        void draw();

    private:
        // Engine memory resource
        static std::unique_ptr<std::pmr::memory_resource> memory_resource;

        // Time point of engine start
        static std::chrono::steady_clock::time_point startTime;
    };

    class DefaultNewDeleteMemoryResource : public std::pmr::memory_resource
    {
    protected:
        void* do_allocate(size_t bytes, size_t alignment) override;
        void do_deallocate(void* p, size_t bytes, size_t alignment) override;
        bool do_is_equal(const memory_resource& other) const noexcept override;
    };
}
#endif
