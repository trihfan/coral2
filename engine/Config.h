#pragma once
#include <thread>

namespace coral
{
    /**
     * @brief The Config struct contains all settings thant can be set for the engine
     * Those settings must be defined before creating the engine
     */
    struct Config
    {
        int threadCount;

        //-----------
        Config() : threadCount(std::max(1, static_cast<int>(std::thread::hardware_concurrency()))) {}
    };
}
