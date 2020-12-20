#include <glad/glad.h>
#include "Engine.h"
#include "Object.h"
#include "Shader.h"
#include "scene/Scene.h"
#include "scene/Node.h"
#include "materials/Material.h"

using namespace coral;

std::unique_ptr<std::pmr::memory_resource> Engine::memory_resource;
RenderParameters Engine::current_parameters;

DEFINE_SINGLETON(Engine)

void Engine::create()
{
    createInstance();
}

void Engine::destroy()
{
    // destroy instances
    SceneManager::destroyInstance();
    ShaderManager::destroyInstance();
    ObjectManager::destroyInstance();
    memory_resource = nullptr;

    // destroy
    destroyInstance();
}

void Engine::setMemoryResource(std::unique_ptr<std::pmr::memory_resource> memory_resource)
{
    Engine::memory_resource = std::move(memory_resource);
}

Engine::Engine()
{
    std::pmr::memory_resource* resource = memory_resource ? memory_resource.get() : std::pmr::new_delete_resource();

    // load backend
    if (!gladLoadGL())
    {
        Logs(error) << "Failed to initialize GLAD";
    }

    // create instances
    ObjectManager::createInstance(resource);
    ShaderManager::createInstance(resource);
    SceneManager::createInstance(resource);
}

void Engine::setCurrentScene(std::shared_ptr<Scene> scene)
{
    SceneManager::setCurrentScene(scene);
}

void Engine::frame()
{
    // update
    ObjectManager::instance->update();
    SceneManager::instance->update();

    // draw
    for (auto camera : SceneManager::instance->cameras)
    {
        current_parameters.camera = camera;
        instance->cull();
        instance->draw();
    }
}

void Engine::cull()
{
    // clear queues
    SceneManager::instance->render_queues.clear();

    // fill queues with visible nodes
    traverse(SceneManager::instance->current_scene->getTopNode(), [](std::shared_ptr<Node> node)
    {
        SceneManager::instance->render_queues[node->getRenderQueue()].nodes.push_back(node);
        return true;
    });
}

void Engine::draw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // for each render pass
    for (auto& queue : SceneManager::instance->render_queues)
    {
        for (auto node : queue.second.nodes)
        {
            if (node->getMaterial())
            {
                node->getMaterial()->getShader()->use();
                node->getMaterial()->use(current_parameters);
                node->draw(current_parameters);
            }
        }
    }
}