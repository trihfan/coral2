#pragma once
#include "Pipeline.h"
#include "Handle.h"
#include <map>
#include <vector>

namespace coral
{
    class PipelineManager
    {
    public:
        void clear();
        Handle<Pipeline> getPipelineByName(const std::string& name);
        Handle<Pipeline> createPipeline(PipelineParams params);

    protected:
        std::vector<std::pair<PipelineParams, Handle<Pipeline>>> pipelines;
        std::vector<Handle<Pipeline>> pipelinesToAssign;
    };
}
