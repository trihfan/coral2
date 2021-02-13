#ifndef ENGINE_H
#define ENGINE_H

#include "EngineConfig.h"
#include "memory_resource.h"
#include "utils/Singleton.h"
#include <chrono>
#include <memory>
#include <vector>

namespace coral
{
    class Scene;
    class Camera;
    class Node;
    class RenderPass;

    /**
     * @brief Parameters for the rendering
     */
    struct RenderParameters
    {
        // Scene
        std::shared_ptr<Camera> camera;
        std::vector<std::shared_ptr<Node>> lights;

        // Time
        double time;
        double deltaTime;

        // Viewport
        int width;
        int height;
    };

    /**
     * @brief Main class of the engine
     */
    class Engine
    {
        MAKE_SINGLETON(Engine)
    public:
        /**
         * @brief Resize the pipeline
         */
        static void resize(int width, int height);

        /**
         * @brief Render a frame
         */
        static void frame();

    private:
        /**
         * @brief Constructor
         * @param config The engine config used to create the render graph
         */
        Engine(const EngineConfig& config = EngineConfig());

    private:
        // Time point of engine start
        std::chrono::steady_clock::time_point startTime;

        // Memory resource used by the engine
        std::shared_ptr<std::pmr::memory_resource> memoryResource;

        // Current render parameters
        RenderParameters currentParameters;
    };
}
#endif
