#pragma once
#include "BackendCapabilities.h"
#include "BackendObjectFactory.h"
#include <memory>

namespace backend
{
    class BackendPipeline;
    struct BackendPipelineParams;
    class Backend
    {
    public:
        Backend() = default;
        virtual ~Backend() = default;

        virtual bool init() = 0;
        virtual bool destroy() = 0;
        virtual bool resize(int width, int height) = 0;
        virtual BackendCapabilities capabilities() const = 0;

        static Backend* current();

    protected:
        static void setCurrent(Backend* backend);

    private:
        static Backend* currentBackend;
    };
}
