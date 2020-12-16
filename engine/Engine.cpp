#include "Engine.h"
#include "Object.h"
#include "Shader.h"
#include "scene/Scene.h"

using namespace coral;

std::unique_ptr<std::pmr::memory_resource> Engine::memory_resource;

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
}
