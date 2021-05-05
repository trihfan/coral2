#pragma once

#include "Backend.h"
#include "RenderParameters.h"
#include "base/Ptr.h"
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
         * @brief Resize the render
         */
        static void resize(int width, int height);

        /**
         * @brief Render a frame
         */
        static void frame();

    private:
        // Time point of engine start
        std::chrono::steady_clock::time_point startTime;

        // Renderer backend
        ptr<backend::Backend> backend;

        // Current render parameters
        RenderParameters currentParameters;

        // Constructor
        Engine(ptr<backend::Backend> backend, int width, int height);

        void resizeInternal(int width, int height);
    };
}
