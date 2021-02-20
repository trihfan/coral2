#include "Engine.h"
#include "EngineConfig.h"
#include "Object.h"
#include "ObjectFactory.h"
#include "materials/Material.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassFramebufferManager.h"
#include "renderpasses/RenderPassManager.h"
#include "renderpasses/RenderPassResourceManager.h"
#include "resources/Shader.h"
#include "scene/DrawableNode.h"
#include "scene/Node.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/camera/Camera.h"
#include <new>

using namespace coral;

void RenderParameters::clear()
{
    camera = nullptr;
    lights = LightArray();
}

DEFINE_SINGLETON(Engine)

Engine::Engine(std::shared_ptr<Backend> backend)
    : backend(std::move(backend))
{
    startTime = std::chrono::steady_clock::now();

    // Create backend
    this->backend->init();

    // create instances
    ObjectFactory::create();
    ShaderManager::create();
    SceneManager::create();
    RenderPassManager::create();
    RenderPassFramebufferManager::create();
    RenderPassResourceManager::create();

    // Setup engine
    EngineConfig::setup();
}

void Engine::release()
{
    SceneManager::destroy();
    ShaderManager::destroy();
    ObjectFactory::destroy();

    backend->destroy();
}

void Engine::resize(int width, int height)
{
    instance->currentParameters.width = width;
    instance->currentParameters.height = height;
    RenderPassManager::invalidate();
}

void Engine::frame()
{
    auto& currentParameters = instance->currentParameters;

    // Update time
    double lastTime = currentParameters.time;
    currentParameters.time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - instance->startTime).count() / 1e6;
    currentParameters.deltaTime = currentParameters.time - lastTime;

    // Update managers
    SceneManager::update();
    RenderPassManager::update(currentParameters);
    //PipelineManager::update();
    ObjectFactory::update(); // finish with object manager update to allocate the gl data

    // Draw
    for (size_t i = 0; i < SceneManager::getCameras().size(); i++)
    {
        currentParameters.camera = SceneManager::getCameras()[i];

        // Cull
        auto queues = SceneManager::buildRenderQueuesFor(currentParameters);

        // Draw
        for (auto& renderpass : RenderPassManager::getOrderedRenderPasses())
        {
            auto it = queues.find(renderpass->getName());
            renderpass->render(it->second, currentParameters);
        }

        currentParameters.clear();
    }
}
