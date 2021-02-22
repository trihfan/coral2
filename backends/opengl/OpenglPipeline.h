#pragma once
#include "BackendPipeline.h"

namespace coral
{
    class Shader;

    class OpenglPipeline : public BackendPipeline
    {
    public:
        OpenglPipeline(const BackendPipelineParams& params);
        void use() override;

        void setUniform(const std::string& name, bool value) const override;
        void setUniform(const std::string& name, int value) const override;
        void setUniform(const std::string& name, float value) const override;
        void setUniform(const std::string& name, const glm::vec2& value) const override;
        void setUniform(const std::string& name, const glm::vec3& value) const override;
        void setUniform(const std::string& name, const glm::vec4& value) const override;
        void setUniform(const std::string& name, const glm::mat2& mat) const override;
        void setUniform(const std::string& name, const glm::mat3& mat) const override;
        void setUniform(const std::string& name, const glm::mat4& mat) const override;

    private:
        std::unique_ptr<Shader> shader;
    };
}
#pragma once
