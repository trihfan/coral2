#include "Engine.h"
#include "Object.h"
#include "Shader.h"

using namespace coral;

std::unique_ptr<std::pmr::memory_resource> Engine::memory_resource;

DEFINE_SINGLETON(Engine)

void Engine::create()
{
    // create default memory resource if none provided
    if (!memory_resource)
    {
        memory_resource = std::unique_ptr<std::pmr::memory_resource>(std::pmr::new_delete_resource());
    }

    // create the engine
    createInstance();
}

void Engine::destroy()
{
    // destroy instance
    ShaderManager::destroyInstance();
    ObjectManager::destroyInstance();
    memory_resource = nullptr;

    // destroy
    destroyInstance();
}

Engine::Engine()
{
    // create instances
    ObjectManager::createInstance(memory_resource.get());
    ShaderManager::createInstance(memory_resource.get());
}

