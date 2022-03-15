#include "Engine.h"
#include "scene/SceneManager.h"
#include "scene/NodeManager.h"
/*#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassFramebufferManager.h"
#include "renderpasses/RenderPassManager.h"
#include "renderpasses/RenderPassResourceManager.h"
#include "resources/PipelineManager.h"
#include "scene/SceneManager.h"*/
#include "resources/stb_image.h"
#include "utils/Freetype.h"
#include "utils/Logs.h"

using namespace coral;
using namespace std::chrono;

Engine::Engine(int width, int height)
{
    // Time
    startTime = std::chrono::steady_clock::now();

    // Create modules
    sceneManager = std::make_unique<SceneManager>();
    nodeManager = std::make_unique<NodeManager>();
    freetype = std::make_unique<Freetype>();

    // Setup stb image
    stbi_set_flip_vertically_on_load(true);

    // Finally resize
    resize(width, height);
}

 Engine::~Engine()
{
    // Delete modules
    //Freetype::release();
    sceneManager = nullptr;
    nodeManager = nullptr;
    freetype = nullptr;
}

void Engine::resize(int width, int height)
{
    Logs(info) << "update size: " << width << ", " << height;
    renderParameters.width = width;
    renderParameters.height = height;
    //RenderPassManager::invalidate();
    //PipelineManager::clear();
}

void Engine::frame()
{
    // Update time
    double lastTime = renderParameters.time;
    renderParameters.time = static_cast<double>(duration_cast<microseconds>(Clock::now() - startTime).count()) / 1e6;
    renderParameters.deltaTime = renderParameters.time - lastTime;

    // Update the scene, lists cameras, lights, ...
    sceneManager->update();

    // Initialize / release all objects, this allocate and deallocate gpu data
    nodeManager->update();

    // Render for each active camera
    for (size_t i = 0; i < sceneManager->getCameras().size(); i++)
    {
        renderParameters.camera = sceneManager->getCameras()[i];

        // Cull for current camera
        auto queues = sceneManager->buildRenderQueuesFor(renderParameters);

        // Render each renderpasses
        /*for (auto& renderpass : RenderPassManager::getOrderedRenderPasses())
        {
            auto it = queues.find(renderpass->getName());
            renderpass->render(it->second, renderParameters);
        }*/

        renderParameters.camera = nullptr;
        renderParameters.lights = LightArray();
    }

    /*backend->beginFrame();

    // Bake the render graph when invalidated
    RenderPassManager::update(currentParameters);

    // Bind pipeline to the render passes
    PipelineManager::update();

    // Update the scene, lists cameras, lights, ...
    SceneManager::update(currentParameters);

    // Initialize / release all objects
    // This allocate and deallocate gpu data
    ObjectFactory::update();

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

    backend->endFrame();*/
}
