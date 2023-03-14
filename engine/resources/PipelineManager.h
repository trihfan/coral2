#pragma once
#include "Pipeline.h"
#include "PropertyArray.h"

namespace coral
{
    class PipelineManager
    {
    public:
        // Properties
        PropertyArray<std::shared_ptr<Pipeline>> pipelines;

        // Get a pipeline by its name
        std::shared_ptr<Pipeline> getPipelineByName(const std::string& name);

        // Create a new pipeline
        std::shared_ptr<Pipeline> createPipeline(PipelineParams params);
    };
}
