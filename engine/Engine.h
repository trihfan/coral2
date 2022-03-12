#pragma once

#include "RenderParameters.h"
#include "utils/Singleton.h"
#include <chrono>

namespace coral
{
    /**
     * @brief Main class of the engine
     */
    class Engine : public Singleton<Engine>
    {
    public:
        // Resize the engine output
        void resize(int width, int height);

        // Draw a frame
        void frame();

    private:
        // Time point of engine start
        std::chrono::steady_clock::time_point startTime;

        // Current render parameters
        RenderParameters currentParameters;

        // Constructor
        Engine(int width, int height);
    };
}
