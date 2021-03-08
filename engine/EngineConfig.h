#pragma once

#include <thread>

namespace coral
{
    struct EngineParameters
    {
        int threadCount = std::max(1, static_cast<int>(std::thread::hardware_concurrency()));
    };

    // config to use for the engine
    // simplify the engine pipeline creation
    class EngineConfig
    {
    public:
        static EngineParameters parameters;
        static void setup();
    };
}
