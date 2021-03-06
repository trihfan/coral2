#pragma once

#include "Pipeline.h"
#include "Singleton.h"
#include "base/Ptr.h"
#include <map>
#include <vector>

namespace coral
{
    class PipelineManager
    {
        MAKE_SINGLETON(PipelineManager)
    public:
        static void clear();
        static void update();
        static ptr<Pipeline> getPipelineByName(const std::string& name);
        static ptr<Pipeline> createPipeline(PipelineParams params);

    protected:
        PipelineManager();

        std::vector<std::pair<PipelineParams, ptr<Pipeline>>> pipelines;
        std::vector<ptr<Pipeline>> pipelinesToAssign;
    };
}
