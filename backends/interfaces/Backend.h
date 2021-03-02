#pragma once
#include "BackendCapabilities.h"
#include "BackendObjectFactory.h"
#include <memory>

namespace backend
{
    struct BackendParams
    {
        int threadCount;
    };

    class Backend
    {
    public:
        Backend() = default;
        virtual ~Backend() = default;

        bool init(const BackendParams& params);
        bool release();

        virtual bool resize(int width, int height) = 0;
        virtual BackendCapabilities capabilities() const = 0;

        static Backend* current();

    protected:
        static void setCurrent(Backend* backend);
        virtual bool internalInit() = 0;
        virtual bool internalRelease() = 0;

    private:
        static Backend* currentBackend;
    };
}
