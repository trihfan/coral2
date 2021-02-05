#include <glad/glad.h>
#include <new>
#include "Engine.h"
#include "Object.h"
#include "Shader.h"
#include "scene/Scene.h"
#include "scene/Node.h"
#include "scene/DrawableNode.h"
#include "materials/Material.h"

using namespace coral;

const char * getGLErrorStr(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR:          return "No error";
    case GL_INVALID_ENUM:      return "Invalid enum";
    case GL_INVALID_VALUE:     return "Invalid value";
    case GL_INVALID_OPERATION: return "Invalid operation";
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
    case GL_OUT_OF_MEMORY:     return "Out of memory";
    default:                   return "Unknown error";
    }
}

void checkGLError()
{
    while (true)
    {
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;

        Logs(error) << "gl error: " << getGLErrorStr(err);
    }
}

std::unique_ptr<std::pmr::memory_resource> Engine::memory_resource;
std::chrono::steady_clock::time_point Engine::startTime;
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
    startTime = std::chrono::steady_clock::now();
    if (!Engine::memory_resource)
    {
        Engine::memory_resource = std::make_unique<DefaultNewDeleteMemoryResource>();
    }
    std::pmr::memory_resource* resource = memory_resource.get();

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
    // update time
    double lastTime = current_parameters.time;
    current_parameters.time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startTime).count() / 1e6;
    current_parameters.deltaTime = current_parameters.time - lastTime;
    
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
        if (node->isDrawable())
        {
            auto drawableNode = node->toHandle<DrawableNode>();
            auto& render_queue = SceneManager::instance->render_queues[drawableNode->getRenderQueue()];
            render_queue.nodes.push_back(drawableNode);
            render_queue.shader_map[drawableNode->getMaterial()->getShader()].insert(drawableNode->getMaterial());
            render_queue.material_map[drawableNode->getMaterial()].push_back(drawableNode);
        }
        return true;
    });
    checkGLError();
}

void Engine::draw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);

    // for each render pass
    for (auto& queue : SceneManager::instance->render_queues)
    {
        // for each shader
        for (auto& shader_pair : queue.second.shader_map)
        {
            // Set up shader
            shader_pair.first->use();

            // for each material
            for (auto material : shader_pair.second)
            {
                // Set up material
                material->use(current_parameters);

                // draw each node
                for (auto node : queue.second.material_map[material])
                {
                    node->draw(current_parameters);
                }
            }
        }
    }
    checkGLError();
}

void* DefaultNewDeleteMemoryResource::do_allocate(size_t bytes, size_t alignment)
{
    return ::operator new(bytes, std::align_val_t(alignment));
}

void DefaultNewDeleteMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment)
{
    ::operator delete(p, bytes, std::align_val_t(alignment));
}

bool DefaultNewDeleteMemoryResource::do_is_equal(const memory_resource& other) const noexcept
{
    return this == &other;
}