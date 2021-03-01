#include "OpenglPipeline.h"
#include "FileUtils.h"
#include "OpenglShader.h"
#include <filesystem>

#define GL_ENABLE_OR_DISABLE(WHAT, ENABLE) \
    if (ENABLE)                            \
    {                                      \
        glEnable(WHAT);                    \
    }                                      \
    else                                   \
    {                                      \
        glDisable(WHAT);                   \
    }

using namespace backend::opengl;
using namespace coral;

OpenglPipeline::OpenglPipeline(const BackendPipelineParams& params)
    : BackendPipeline(params)
{
    shader = std::make_unique<OpenglShader>();
    shader->addShaderData(OpenglShader::vertex, FileUtils::readAll(params.vertexShaderFile));
    shader->addShaderData(OpenglShader::fragment, FileUtils::readAll(params.fragmentShaderFile));
    shader->init();
}

void OpenglPipeline::use()
{
    shader->use();

    GL_ENABLE_OR_DISABLE(GL_DEPTH_TEST, params.depthTest)
    GL_ENABLE_OR_DISABLE(GL_CULL_FACE, params.cullFace != CullFace::none);
    glCullFace(params.cullFace == CullFace::front ? GL_FRONT : GL_BACK);
}

void OpenglPipeline::resize(int, int)
{
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
