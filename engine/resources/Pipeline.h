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
    struct PipelineParams
    {
        std::string renderpass;
        backend::BackendPipelineParams params;
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

        // Return true if the pipeline should be updated
        bool isDirty() const;
        void setDirty();

        template <typename T>
        void setUniform(const std::string& name, const T& value) { backendPipeline->setUniform(name, value); }

        virtual void init() override;
        virtual void release() override;

    private:
        const PipelineParams params;
        std::unique_ptr<backend::BackendPipeline> backendPipeline;
        bool dirty;
    };
}
