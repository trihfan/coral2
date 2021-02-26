#include "Pipeline.h"
#include "BackendPipeline.h"
#include "Engine.h"

using namespace coral;

bool PipelineParams::operator==(const PipelineParams& other) const
{
    return renderpass == other.renderpass
        && vertexShaderFile == other.vertexShaderFile
        && fragmentShaderFile == other.fragmentShaderFile;
}

Pipeline::Pipeline(const PipelineParams& params)
    : params(params)
    , resized(true)
{
    connect<&Pipeline::init>(Object::init, this);
    connect<&Pipeline::release>(Object::release, this);
}

const std::string& Pipeline::getRenderPassName() const
{
    return params.renderpass;
}

void Pipeline::use()
{
    backendPipeline->use();
    if (resized)
    {
        backendPipeline->resize(width, height);
    }
}

void Pipeline::resize(int width, int height)
{
    resized = true;
    this->width = width;
    this->height = height;
}

void Pipeline::init()
{
    backend::BackendPipelineParams params;
    params.vertexShaderFile = this->params.vertexShaderFile;
    params.fragmentShaderFile = this->params.fragmentShaderFile;
    backendPipeline = Engine::getBackend().createPipeline(params);
}

void Pipeline::release()
{
    backendPipeline = nullptr;
}
