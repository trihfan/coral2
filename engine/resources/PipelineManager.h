#pragma once

#include "Pipeline.h"
#include "utils/Singleton.h"
#include <map>

namespace coral
{
    class PipelineManager
    {
        MAKE_SINGLETON(PipelineManager)
    public:
        static void resize(int width, int height);
        static void update();
        static Handle<Pipeline> getPipeline(const PipelineParams& params);

    protected:
        PipelineManager() = default;

        std::vector<std::pair<PipelineParams, Handle<Pipeline>>> pipelines;
        std::vector<Handle<Pipeline>> pipelinesToAssign;
    };
}
