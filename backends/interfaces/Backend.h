#pragma once
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

        // Creation
        virtual std::unique_ptr<BackendPipeline> createPipeline(const BackendPipelineParams& params) const = 0;
    };
}
