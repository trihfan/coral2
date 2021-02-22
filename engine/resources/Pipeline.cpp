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
}

void Pipeline::init()
{
    BackendPipelineParams params;
    params.vertexShaderFile = this->params.vertexShaderFile;
    params.fragmentShaderFile = this->params.fragmentShaderFile;
    backendPipeline = Engine::getBackend().createPipeline(params);
}

void Pipeline::release()
{
    backendPipeline = nullptr;
}
