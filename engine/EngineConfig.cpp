#include "EngineConfig.h"
#include "MemoryResource.h"
#include "Object.h"
#include "renderpasses/RenderPassManager.h"
#include "renderpasses/RenderPassDefault.h"
#include "renderpasses/RenderPassPresentation.h"

using namespace coral;

EngineConfig::EngineConfig()
{
    memoryResource = std::make_shared<DefaultNewDeleteMemoryResource>();
}

void EngineConfig::setup()
{
    RenderPassManager::setDefaultRenderPass(ObjectManager::createWithName<RenderPassDefault>("defaultrenderpass"));
}
