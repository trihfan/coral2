#pragma once

#include "BackendPipeline.h"
#include "Object.h"
#include <memory>

namespace coral
{
    /**
     * @brief The PipelineParams contains the creation parameters 
     * for the pipeline
     */
    struct PipelineParams : backend::BackendPipelineParams
    {
        // Corresponding renderpass
        std::string renderpass;

        backend::BackendPipelineParams params;

        // Comparison
        bool operator==(const PipelineParams& other) const;
    };

    /**
     * @brief The Pipeline class describe a pipeline configuration for rendering
     */
    class Pipeline : public Object
    {
    public:
        Pipeline(const PipelineParams& params);
        const std::string& getRenderPassName() const;
        void use();

        template <typename T>
        void setUniform(const std::string& name, const T& value) { backendPipeline->setUniform(name, value); }

    private:
        const PipelineParams params;
        std::unique_ptr<backend::BackendPipeline> backendPipeline;

        void init();
        void release();
    };
}
