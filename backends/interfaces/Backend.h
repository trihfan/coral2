#pragma once
#include "BackendCapabilities.h"
#include "BackendObjectFactory.h"
#include <memory>

namespace backend
{
    /**
     * @brief Backend initialization parameters
     */
    struct BackendParams
    {
        int threadCount;
    };

    /**
     * @brief The Backend main class interface
     */
    class Backend
    {
    public:
        // Construction
        Backend() = default;
        virtual ~Backend() = default;

        // Backend initialization
        bool init(const BackendParams& params);
        bool release();

        // Backend run
        virtual bool resize(int width, int height) = 0;
        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        // Backend information
        virtual std::string getName() const = 0;
        virtual BackendCapabilities capabilities() const = 0;

    protected:
        size_t threadCount = 1;

        // Backend initialization
        virtual bool internalInit() = 0;
        virtual bool internalRelease() = 0;
    };
}
