#pragma once

#include "BackendPipeline.h"
#include "Object.h"
#include <memory>

namespace coral
{
    struct PipelineParams
    {
        std::string renderpass;
        std::string vertexShaderFile;
        std::string fragmentShaderFile;

        bool operator==(const PipelineParams& other) const;
    };

    class Pipeline : public Object
    {
    public:
        Pipeline(const PipelineParams& params);
        const std::string& getRenderPassName() const;
        void use();

        template <typename T>
        void setUniform(const std::string& name, const T& value) { backendPipeline->setUniform(name, value); }

    private:
        PipelineParams params;
        std::unique_ptr<BackendPipeline> backendPipeline;

        void init();
        void release();
    };
}
