#include "EngineConfig.h"
#include "Object.h"
#include "ObjectFactory.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassDefault.h"
#include "renderpasses/RenderPassManager.h"
#include "renderpasses/RenderPassPresentation.h"
#include "resources/Pipeline.h"
#include "resources/Resource.h"

using namespace coral;

EngineParameters EngineConfig::parameters;

void EngineConfig::setup()
{
    RenderPassResource backbuffer { "backbuffer", backend::BackendFramebufferResourceRole::color, backend::BackendResourceFormat::r8g8b8a8u, 1 };
    RenderPassResource depth { "depth", backend::BackendFramebufferResourceRole::depth, backend::BackendResourceFormat::depth24_pencil8, 1 };
    RenderPassResource stencil { "depth", backend::BackendFramebufferResourceRole::stencil, backend::BackendResourceFormat::depth24_pencil8, 1 };

    // default
    auto defaultRenderPass = ObjectFactory::createWithName<RenderPassDefault>(coral::defaultRenderPassName);
    defaultRenderPass->addOutput(backbuffer);
    defaultRenderPass->addOutput(depth);
    defaultRenderPass->addOutput(stencil);
    RenderPassManager::addRenderPass(defaultRenderPass);

    // presentation
    auto presentation = ObjectFactory::createWithName<RenderPassPresentation>("presentationRenderPass");
    presentation->addInput(backbuffer);
    RenderPassManager::addRenderPass(presentation);
}
