#include "Pipeline.h"
#include "BackendPipeline.h"
#include "Engine.h"

using namespace coral;

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
