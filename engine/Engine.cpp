#include "Engine.h"
#include "Object.h"
#include "Shader.h"

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
}

void Engine::frame()
{

}
