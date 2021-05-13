#pragma once

#include "BackendPipeline.h"
#include "base/Object.h"
#include <memory>

namespace coral
{
    /**
     * @brief The Pipeline class describe a pipeline configuration for rendering
     */
    class Pipeline : public Object
    {
    public:
        Pipeline(const std::string& renderpass, const backend::BackendPipelineParams& params);
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
        std::string renderpass;
        const backend::BackendPipelineParams params;
        std::unique_ptr<backend::BackendPipeline> backendPipeline;
        bool dirty;
    };
}
