#pragma once

#include "Backend.h"
#include "RenderParameters.h"
#include "utils/Singleton.h"
#include <chrono>
#include <memory>

namespace backend
{
    class Backend;
}

namespace coral
{
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

        /**
         * @brief Return the backend
         */
        static backend::Backend& getBackend();

    private:
        /**
         * @brief Constructor
         * @param config The engine config used to create the render graph
         */
        Engine(std::shared_ptr<backend::Backend> backend);

    private:
        // Time point of engine start
        std::chrono::steady_clock::time_point startTime;

        // Renderer backend
        std::shared_ptr<backend::Backend> backend;

        // Current render parameters
        RenderParameters currentParameters;
    };
}
