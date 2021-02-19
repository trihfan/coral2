#pragma once

#include "RenderParameters.h"
#include "backend/interfaces/Backend.h"
#include "utils/Singleton.h"
#include <chrono>
#include <memory>

namespace coral
{
    class Backend;

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
        Engine(std::shared_ptr<Backend> backend);

    private:
        // Time point of engine start
        std::chrono::steady_clock::time_point startTime;

        // Renderer backend
        std::shared_ptr<Backend> backend;

        // Current render parameters
        RenderParameters currentParameters;
    };
}
