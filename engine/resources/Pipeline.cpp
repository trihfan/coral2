#include "Pipeline.h"
#include "BackendPipeline.h"
#include "Engine.h"

using namespace coral;

bool PipelineParams::operator==(const PipelineParams& other) const
{
    return renderpass == other.renderpass
        && vertexShaderFile == other.vertexShaderFile
        && fragmentShaderFile == other.fragmentShaderFile
        && depthTest == other.depthTest
        && cullFace == other.cullFace;
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
    backend::BackendPipelineParams bParams = params;
    backendPipeline = backend::BackendObjectFactory<backend::BackendPipeline>::create(bParams);
}

void Pipeline::release()
{
    backendPipeline = nullptr;
}
