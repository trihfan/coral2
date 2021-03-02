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
        static void clear();
        static void update();
        static Handle<Pipeline> getPipeline(const PipelineParams& params);

    protected:
        PipelineManager();

        std::vector<std::pair<PipelineParams, Handle<Pipeline>>> pipelines;
        std::vector<Handle<Pipeline>> pipelinesToAssign;
    };
}
