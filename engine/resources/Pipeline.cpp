#include "Pipeline.h"
#include "BackendPipeline.h"
#include "Engine.h"

using namespace coral;

bool PipelineParams::operator==(const PipelineParams& other) const
{
    return renderpass == other.renderpass
        && params.vertexShaderFile == other.params.vertexShaderFile
        && params.fragmentShaderFile == other.params.fragmentShaderFile
        && params.depthTest == other.params.depthTest
        && params.cullFace == other.params.cullFace
        && params.blending == other.params.blending;
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
    backendPipeline = backend::BackendObjectFactory<backend::BackendPipeline>::create(params.params);
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
