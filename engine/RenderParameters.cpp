#include "Engine.h"
#include "Object.h"
#include "ObjectManager.h"
#include "materials/Material.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassFramebufferManager.h"
#include "renderpasses/RenderPassManager.h"
#include "renderpasses/RenderPassResourceManager.h"
#include "resources/Shader.h"
#include "scene/DrawableNode.h"
#include "scene/Node.h"
#include "scene/SceneManager.h"
#include "utils/Error.h"
#include <glad/glad.h>
#include <new>

using namespace coral;

DEFINE_SINGLETON(Engine)

Engine::Engine(const EngineConfig& config)
{
    startTime = std::chrono::steady_clock::now();
    memoryResource = *config.memoryResource;

    // load backend
    if (!gladLoadGL())
    {
        Logs(error) << "Failed to initialize GLAD";
    }

    // create instances
    ObjectManager::create(*config.memoryResource);
    ShaderManager::create(*config.memoryResource);
    SceneManager::create(*config.memoryResource);
    RenderPassManager::create(*config.memoryResource);
    RenderPassFramebufferManager::create(*config.memoryResource);
    RenderPassResourceManager::create(*config.memoryResource);

    // default config
    config.setup();
}

void Engine::release()
{
    SceneManager::destroy();
    ShaderManager::destroy();
    ObjectManager::destroy();
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
    double lastTime
        = currentParameters.time;
    currentParameters.time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - instance->startTime).count() / 1e6;
    currentParameters.deltaTime = currentParameters.time - lastTime;

    // Update managers
    currentParameters.camera = nullptr;
    SceneManager::update(currentParameters);
    RenderPassManager::update(currentParameters);
    ObjectManager::update(); // finish with object manager update to allocate the gl data

    // Draw
    for (auto camera : SceneManager::getCameras())
    {
        currentParameters.camera = camera;

        // Cull
        auto queues = SceneManager::buildRenderQueuesFor(currentParameters);

        // Draw
        for (auto& renderpass : RenderPassManager::getOrderedRenderPasses())
        {
            auto it = queues.find(renderpass->getName());
            renderpass->render(it->second, currentParameters);
        }
    }
}