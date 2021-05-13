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
    , depthTest(params.depthTest)
    , cullFace(params.cullFace)
    , blending(params.blending)
{
    shader = std::make_unique<OpenglShader>(params.name);
    shader->addShaderCode(vertex, params.shader.vertexShader);
    shader->addShaderCode(fragment, params.shader.fragmentShader);
    shader->init();
}

void OpenglPipeline::use()
{
    shader->use();

    GL_ENABLE_OR_DISABLE(GL_DEPTH_TEST, depthTest)

    GL_ENABLE_OR_DISABLE(GL_CULL_FACE, cullFace != CullFace::none)
    glCullFace(cullFace == CullFace::front ? GL_FRONT : GL_BACK);

    GL_ENABLE_OR_DISABLE(GL_BLEND, blending)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
