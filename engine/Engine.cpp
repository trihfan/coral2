#include "Engine.h"
#include "AssetManager.h"
#include "BackendCommandBuffer.h"
#include "EngineConfig.h"
#include "Object.h"
#include "ObjectFactory.h"
#include "materials/Material.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassFramebufferManager.h"
#include "renderpasses/RenderPassManager.h"
#include "renderpasses/RenderPassResourceManager.h"
#include "resources/PipelineManager.h"
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

Engine::Engine(std::shared_ptr<backend::Backend> backend)
    : backend(std::move(backend))
{
    startTime = std::chrono::steady_clock::now();
    AssetManager::init();

    // Init backend
    backend::BackendParams params;
    params.threadCount = EngineConfig::parameters.threadCount;
    this->backend->init(params);

    // create instances
    ObjectFactory::create();
    PipelineManager::create();
    SceneManager::create();
    RenderPassManager::create();
    RenderPassFramebufferManager::create();
    RenderPassResourceManager::create();

    // Setup engine
    EngineConfig::setup();
}

void Engine::release()
{
    // Destroy instances
    RenderPassResourceManager::destroy();
    RenderPassFramebufferManager::destroy();
    RenderPassManager::destroy();
    SceneManager::destroy();
    PipelineManager::destroy();
    ObjectFactory::destroy();

    // Release backend
    backend->release();
}

void Engine::resize(int width, int height)
{
    instance->currentParameters.width = width;
    instance->currentParameters.height = height;
    RenderPassManager::invalidate();
    PipelineManager::clear();
}

void Engine::frame()
{
    auto& currentParameters = instance->currentParameters;

    // Update time
    double lastTime = currentParameters.time;
    currentParameters.time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - instance->startTime).count()) / 1e6;
    currentParameters.deltaTime = currentParameters.time - lastTime;

    // Update the scene, lists cameras, lights, ...
    SceneManager::update();

    // Bake the render graph when invalidated
    RenderPassManager::update(currentParameters);

    // Bind pipeline to the render passes
    PipelineManager::update();

    // Initialize / release all objects
    // This allocate and deallocate gpu data
    ObjectFactory::update();

    // Submit objects updates
    backend::BackendCommandBuffer::submit(backend::BackendCommandBufferStage::staging);

    // Render for each active camera
    for (size_t i = 0; i < SceneManager::getCameras().size(); i++)
    {
        currentParameters.camera = SceneManager::getCameras()[i];

        // Cull for current camera
        auto queues = SceneManager::buildRenderQueuesFor(currentParameters);

        // Render each renderpasses
        for (auto& renderpass : RenderPassManager::getOrderedRenderPasses())
        {
            auto it = queues.find(renderpass->getName());
            renderpass->render(it->second, currentParameters);
        }

        currentParameters.clear();
    }

    backend::BackendCommandBuffer::submit(backend::BackendCommandBufferStage::draw);
}
