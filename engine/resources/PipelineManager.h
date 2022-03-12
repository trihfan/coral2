#pragma once

#include "Pipeline.h"
#include "EngineModule.h"
#include "base/Ptr.h"
#include <map>
#include <vector>

namespace coral
{
    class PipelineManager : public EngineModule<PipelineManager>
    {
    public:
        void clear();
        ptr<Pipeline> getPipelineByName(const std::string& name);
        ptr<Pipeline> createPipeline(PipelineParams params);

    protected:
        std::vector<std::pair<PipelineParams, ptr<Pipeline>>> pipelines;
        std::vector<ptr<Pipeline>> pipelinesToAssign;
    };
}
