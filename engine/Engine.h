#pragma once
#include <chrono>
#include "Handle.h"
#include "RenderParameters.h"
#include "Config.h"

namespace coral
{
    using Clock = std::chrono::high_resolution_clock;
    class Config;
    class RenderParameters;
    class SceneManager;
    class NodeManager;
    class Freetype;

    template<typename Type>
    using Module = std::unique_ptr<Type>;

    class Engine;
    class CurrentEngine
    {
    public:
        // Creation
        void init(int width, int height) { engine = std::make_unique<Engine>(width, height); }
        void destroy() { engine = nullptr; }
        Engine* operator->() { return engine.get(); }

    private:
        static std::unique_ptr<Engine> engine;
    };

    inline static CurrentEngine engine;

    /**
     * @brief Main class of the engine
     */
    class Engine
    {
    public:
        //
        Engine(int width, int height);
        ~Engine();

        // Resize the engine output
        void resize(int width, int height);

        // Draw a frame
        void frame();

        // Properties
        Config config;
        RenderParameters renderParameters;

        // Modules
        Module<SceneManager> sceneManager;
        Module<NodeManager> nodeManager;
        Module<Freetype> freetype;

    private:
        // Time point of engine start
        Clock::time_point startTime;
    };
}
