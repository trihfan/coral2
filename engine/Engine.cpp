#include <glad/glad.h>
#include <new>
#include "Engine.h"
#include "Object.h"
#include "resources/Shader.h"
#include "resources/Framebuffer.h"
#include "resources/Resource.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderPassManager.h"
#include "renderpasses/RenderPassDefault.h"
#include "scene/Scene.h"
#include "scene/Node.h"
#include "scene/DrawableNode.h"
#include "materials/Material.h"
#include "utils/Error.h"

using namespace coral;

std::chrono::steady_clock::time_point Engine::startTime;
RenderParameters Engine::current_parameters;

DEFINE_SINGLETON(Engine)

void Engine::create(const EngineConfig& config)
{
    createInstance(config);
}

void Engine::destroy()
{
    // destroy instances
    SceneManager::destroyInstance();
    ShaderManager::destroyInstance();
    ObjectManager::destroyInstance();

    // destroy
    destroyInstance();
}

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
    ObjectManager::createInstance(*config.memoryResource);
    ShaderManager::createInstance(*config.memoryResource);
    SceneManager::createInstance(*config.memoryResource);
    RenderPassManager::createInstance(*config.memoryResource);
    FramebufferManager::createInstance(*config.memoryResource);
    ResourceManager::createInstance(*config.memoryResource);

    // default config
    config.setup();
}

void Engine::setCurrentScene(std::shared_ptr<Scene> scene)
{
    SceneManager::setCurrentScene(scene);
}

void Engine::frame()
{
    // clear any residual opengl error
    CHECK_OPENGL_ERROR

    // update time
    double lastTime = current_parameters.time;
    current_parameters.time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startTime).count() / 1e6;
    current_parameters.deltaTime = current_parameters.time - lastTime;

    // update
    SceneManager::instance->update();
    RenderPassManager::instance->update();
    ObjectManager::instance->update(); // finish with object manager update to allocate the gl data

    // draw
    for (auto camera : SceneManager::instance->cameras)
    {
        current_parameters.camera = camera;
        instance->cull();
        instance->draw();
    }

    // clear any residual opengl error
    CHECK_OPENGL_ERROR
}

void Engine::cull()
{
    // clear queues
    SceneManager::instance->render_queues.clear();

    // init queues
    for (auto& renderpass : RenderPassManager::instance->orderedRenderPasses)
    {
        SceneManager::instance->render_queues.insert(std::make_pair(renderpass->getName(), RenderQueue()));
    }

    // fill queues with visible nodes
    traverse(SceneManager::instance->current_scene->getTopNode(), [](std::shared_ptr<Node> node)
    {
        if (node->isDrawable())
        {
            auto drawableNode = node->toHandle<DrawableNode>();
            for (const auto& id : drawableNode->getRenderQueueTags())
            {
                auto& render_queue = SceneManager::instance->render_queues[id];
                render_queue.nodes.push_back(drawableNode);
                render_queue.shader_map[drawableNode->getMaterial()->getShader()].insert(drawableNode->getMaterial());
                render_queue.material_map[drawableNode->getMaterial()].push_back(drawableNode);
            }
        }
        return true;
    });

    CHECK_OPENGL_ERROR
}

void Engine::draw()
{    
    // for each render pass
    for (auto& renderpass : RenderPassManager::instance->orderedRenderPasses)
    {
        auto it = SceneManager::instance->render_queues.find(renderpass->getName());
        renderpass->render(it->second);
    }
}