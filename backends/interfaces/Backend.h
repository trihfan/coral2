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

        /**
         * @brief Return the current backend
         */
        static Backend* current();

        // Backend initialization
        /**
         * @brief Initialize the backend with the given params
         */
        bool init(const BackendParams& params);

        /**
         * @brief Release the backend
         */
        bool release();

        // Backend information
        /**
         * @brief Return the backend name (ex gl, gles, vulkan, ...)
         */
        virtual std::string getName() const = 0;

        /**
         * @brief Return the backend capabilities
         */
        virtual BackendCapabilities capabilities() const = 0;

        /**
         * @brief Resize backend resources
         */
        virtual bool resize(int width, int height) = 0;

    protected:
        size_t threadCount = 1;

        // Set current backend
        static void setCurrent(Backend* backend);

        // Backend initialization
        virtual bool internalInit() = 0;
        virtual bool internalRelease() = 0;

    private:
        // Current backend
        static Backend* currentBackend;
    };
}
