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
        && cullFace == other.cullFace
        && blending == other.blending;
}

Pipeline::Pipeline(const PipelineParams& params)
    : params(params)
    , dirty(false)
{
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
    Object::init();
    backend::BackendPipelineParams bParams = params;
    backendPipeline = backend::BackendObjectFactory<backend::BackendPipeline>::create(bParams);
}

void Pipeline::release()
{
    Object::release();
    backendPipeline = nullptr;
}

bool Pipeline::isDirty() const
{
    return dirty;
}

void Pipeline::setDirty()
{
    dirty = true;
}
