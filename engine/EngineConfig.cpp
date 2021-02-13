#include "EngineConfig.h"
#include "MemoryResource.h"
#include "Object.h"
#include "ObjectManager.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassDefault.h"
#include "renderpasses/RenderPassManager.h"
#include "renderpasses/RenderPassPresentation.h"

using namespace coral;

EngineConfig::EngineConfig()
{
    memoryResource = std::make_shared<DefaultNewDeleteMemoryResource>();
}

void EngineConfig::setup() const
{
    RenderPassResource backbuffer { "backbuffer", ResourceRole::color, GL_RGBA, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, 1 };
    RenderPassResource depth { "depth", ResourceRole::depth, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 1 };
    RenderPassResource stencil { "depth", ResourceRole::stencil, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 1 };

    // default
    auto defaultRenderPass = ObjectManager::createWithName<RenderPassDefault>(coral::defaultRenderPassName);
    defaultRenderPass->addOutput(backbuffer);
    defaultRenderPass->addOutput(depth);
    defaultRenderPass->addOutput(stencil);
    RenderPassManager::addRenderPass(defaultRenderPass);

    // presentation
    auto presentation = ObjectManager::createWithName<RenderPassPresentation>("presentationRenderPass");
    presentation->addInput(backbuffer);
    RenderPassManager::addRenderPass(presentation);
}
