#include "SceneManager.h"
#include "DrawableNode.h"
#include "Node.h"
#include "Scene.h"
#include "camera/Camera.h"
#include "light/Light.h"
#include "materials/Material.h"
#include "renderpasses/RenderPass.h"
#include <functional>

using namespace coral;

DEFINE_SINGLETON(SceneManager)

void SceneManager::release()
{
}

void SceneManager::update(const RenderParameters& parameters)
{
    // check scene
    if (!instance->currentScene)
    {
        return;
    }

    // list cameras and lights
    instance->cameras.clear();
    instance->lights.clear();

    traverse(
        instance->currentScene->getTopNode(), +[](std::shared_ptr<Node> node) {
            if (node->isA<Camera>())
            {
                instance->cameras.push_back(node->toHandle<Camera>());
            }
            else if (node->isA<Light>())
            {
                instance->lights.push_back(node->toHandle<Light>());
            }
            return true;
        });

    // update
    traverse(
        instance->currentScene->getTopNode(), +[](std::shared_ptr<Node> node) {
            node->update();
            return true;
        });
}

void SceneManager::setCurrentScene(std::shared_ptr<Scene> scene)
{
    instance->currentScene = scene;
    instance->cameras.clear();
}

const std::vector<std::shared_ptr<Camera>>& SceneManager::getCameras()
{
    return instance->cameras;
}

std::unordered_map<std::string, RenderQueue> SceneManager::buildRenderQueuesFor(const RenderParameters& parameters)
{
    std::unordered_map<std::string, RenderQueue> queues;

    // Init queues
    for (auto& renderpass : RenderPassManager::getOrderedRenderPasses())
    {
        queues[renderpass->getName()] = RenderQueue();
    }

    // Fill queues with visible nodes
    traverse(instance->currentScene->getTopNode(), [&queues](std::shared_ptr<Node> node) {
        if (node->isDrawable())
        {
            auto drawableNode = node->toHandle<DrawableNode>();
            for (const auto& id : drawableNode->getRenderQueueTags())
            {
                auto& render_queue = queues[id];
                render_queue.nodes.push_back(drawableNode);
                render_queue.shaderMap[drawableNode->getMaterial()->getShader()].insert(drawableNode->getMaterial());
                render_queue.materialMap[drawableNode->getMaterial()].push_back(drawableNode);
            }
        }
        return true;
    });

    return queues;
}

SceneManager::SceneManager(std::shared_ptr<std::pmr::memory_resource> memoryResource)
{
}