#include "EngineConfig.h"
#include "base/Object.h"
#include "base/ObjectFactory.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassDefault.h"
#include "renderpasses/RenderPassManager.h"
#include "resources/Pipeline.h"
#include "resources/Resource.h"

using namespace coral;

EngineParameters EngineConfig::parameters;

void EngineConfig::setup()
{
    RenderPassResource backbuffer;
    backbuffer.name = RenderPassResource::backbuffer;

    // default
    auto defaultRenderPass = ObjectFactory::createWithName<RenderPassDefault>(coral::defaultRenderPassName);
    defaultRenderPass->addOutput(backbuffer);
    RenderPassManager::addRenderPass(defaultRenderPass);
}
