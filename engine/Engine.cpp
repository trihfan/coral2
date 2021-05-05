#include "Engine.h"
#include "BackendCommandBuffer.h"
#include "EngineConfig.h"
#include "base/ObjectFactory.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassFramebufferManager.h"
#include "renderpasses/RenderPassManager.h"
#include "renderpasses/RenderPassResourceManager.h"
#include "resources/PipelineManager.h"
#include "resources/stb_image.h"
#include "scene/SceneManager.h"
#include "utils/Freetype.h"

using namespace coral;

DEFINE_SINGLETON(Engine)

Engine::Engine(std::shared_ptr<backend::Backend> backend, int width, int height)
    : backend(std::move(backend))
{
    startTime = std::chrono::steady_clock::now();

    // Setup stb image
    stbi_set_flip_vertically_on_load(true);

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
    Freetype::init();
    EngineConfig::setup();

    // Finally resize
    resizeInternal(width, height);
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
    Freetype::release();
    backend->release();
}

void Engine::resize(int width, int height)
{
    instance->resizeInternal(width, height);
}

void Engine::resizeInternal(int width, int height)
{
    Logs(info) << "update size: " << width << ", " << height;
    currentParameters.width = width;
    currentParameters.height = height;
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

    // Bake the render graph when invalidated
    RenderPassManager::update(currentParameters);

    // Bind pipeline to the render passes
    PipelineManager::update();

    // Update the scene, lists cameras, lights, ...
    SceneManager::update(currentParameters);

    // Initialize / release all objects
    // This allocate and deallocate gpu data
    ObjectFactory::update();

    // Submit objects updates
    backend::BackendCommandBufferManager::submit(backend::BackendCommandBufferStage::staging);

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

    backend::BackendCommandBufferManager::submit(backend::BackendCommandBufferStage::draw);
}
