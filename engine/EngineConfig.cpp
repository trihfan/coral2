#include "EngineConfig.h"
#include "MemoryResource.h"
#include "Object.h"
#include "renderpasses/RenderPass.h"
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
    // default
    auto defaultRenderPass = ObjectManager::createWithName<RenderPassDefault>(coral::defaultRenderPassName);
    defaultRenderPass->addOutput(RenderPassOutput{ "backbuffer", ResourceRole::color, GL_RGBA, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, 1 });
    defaultRenderPass->addOutput(RenderPassOutput{ "depth", ResourceRole::depth, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 1 });
    defaultRenderPass->addOutput(RenderPassOutput{ "depth", ResourceRole::stencil, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 1 });
    RenderPassManager::addRenderPass(defaultRenderPass);

    // presentation
    auto presentation = ObjectManager::createWithName<RenderPassPresentation>("presentationRenderPass");
    defaultRenderPass->addInput(RenderPassInput{ "backbuffer" });
    RenderPassManager::addRenderPass(presentation);
}
