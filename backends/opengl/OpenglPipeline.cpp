#include "OpenglPipeline.h"
#include "FileUtils.h"
#include "Shader.h"
#include <filesystem>

using namespace coral;

OpenglPipeline::OpenglPipeline(const BackendPipelineParams& params)
{
    shader = std::make_unique<Shader>();
    shader->addShaderData(Shader::vertex, FileUtils::readAll(params.vertexShaderFile));
    shader->addShaderData(Shader::fragment, FileUtils::readAll(params.fragmentShaderFile));
    shader->init();
}

void OpenglPipeline::use()
{
    shader->use();
}

void OpenglPipeline::setUniform(const std::string& name, bool value) const
{
    shader->setUniform(name, value);
}

void OpenglPipeline::setUniform(const std::string& name, int value) const
{
    shader->setUniform(name, value);
}

void OpenglPipeline::setUniform(const std::string& name, float value) const
{
    shader->setUniform(name, value);
}

void OpenglPipeline::setUniform(const std::string& name, const glm::vec2& value) const
{
    shader->setUniform(name, value);
}

void OpenglPipeline::setUniform(const std::string& name, const glm::vec3& value) const
{
    shader->setUniform(name, value);
}

void OpenglPipeline::setUniform(const std::string& name, const glm::vec4& value) const
{
    shader->setUniform(name, value);
}

void OpenglPipeline::setUniform(const std::string& name, const glm::mat2& mat) const
{
    shader->setUniform(name, mat);
}

void OpenglPipeline::setUniform(const std::string& name, const glm::mat3& mat) const
{
    shader->setUniform(name, mat);
}

void OpenglPipeline::setUniform(const std::string& name, const glm::mat4& mat) const
{
    shader->setUniform(name, mat);
}
