#pragma once
#include "Pipeline.h"
#include "Object.h"
#include "PropertyMap.h"

namespace coral
{
    class PipelineManager
    {
    public:
        // Properties
        PropertyMap<PipelineParams, Object<Pipeline>> pipelines;

        // Get a pipeline by its name
        Object<Pipeline> getPipelineByName(const std::string& name);

        // Create a new pipeline
        Object<Pipeline> createPipeline(PipelineParams params);
    };
}
