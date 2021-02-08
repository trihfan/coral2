#ifndef ENGINE_H
#define ENGINE_H

#ifdef __APPLE__
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#else
    #include <memory_resource>
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
    class RenderPass;
    class DefaultNewDeleteMemoryResource;

    struct RenderParameters
	{
        std::shared_ptr<Camera> camera;
        std::vector<std::shared_ptr<Node>> lights;
        double time;
        double deltaTime;
	};

    enum DefaultEngineRenderPass
    {
        defaultRenderPass = 1000,
        transparentRenderPass = 2000
    };

    enum Transparency { none, simple, depthPeeling, linkedList };
    struct EngineConfig
    {
        // constructor with default config
        EngineConfig();

        // parameters
        std::shared_ptr<std::pmr::memory_resource> memoryResource;
        Transparency transparency;
    };

    class Engine
    {
        MAKE_SINGLETON(Engine)
    public:
        // render parameters for the current frame
        static RenderParameters current_parameters;

        // creation
        static void create(const EngineConfig& config = EngineConfig());
        static void destroy();

        // run
        static void setCurrentScene(std::shared_ptr<Scene> scene);
        static void frame();

    private:
        // Constructor
        Engine(const EngineConfig& config);

        void cull();
        void draw();

    private:
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
