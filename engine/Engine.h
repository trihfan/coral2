#ifndef ENGINE_H
#define ENGINE_H

#ifdef __APPLE__
    #include <experimental/memory_resource>
    namespace std { namespace pmr = experimental::pmr; }
#else
    #include <memory_resource>
#endif

#include "EngineConfig.h"
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

        // Memory resource used by the engine
        std::shared_ptr<std::pmr::memory_resource> memoryResource;
    };
}
#endif
