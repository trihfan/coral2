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

void EngineConfig::setup() const
{
    auto defaultRenderPass = ObjectManager::createWithName<RenderPassDefault>(coral::defaultRenderPassName);
    defaultRenderPass->addOutput(RenderPassResource{ "backbuffer" });
    RenderPassManager::addRenderPass(defaultRenderPass);

    auto presentation = ObjectManager::createWithName<RenderPassPresentation>("presentationRenderPass");
    defaultRenderPass->addInput(RenderPassResource{ "backbuffer" });
    defaultRenderPass->addOutput(RenderPassResource{ "screen" });
    RenderPassManager::addRenderPass(presentation);
}
