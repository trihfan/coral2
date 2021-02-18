#include "SceneManager.h"
#include "DrawableNode.h"
#include "Node.h"
#include "RenderParameters.h"
#include "Scene.h"
#include "camera/Camera.h"
#include "light/PointLight.h"
#include "materials/Material.h"
#include "renderpasses/RenderPass.h"
#include <functional>

using namespace coral;

DEFINE_SINGLETON(SceneManager)

void SceneManager::release()
{
}

void SceneManager::update()
{
    // check scene
    if (!instance->currentScene)
    {
        return;
    }

    // list cameras
    instance->cameras.clear();
    instance->lights.clear();

    traverse(
        instance->currentScene->getTopNode(), +[](Handle<Node> node) {
            if (node->isA<Camera>())
            {
                instance->cameras.push_back(node->toHandle<Camera>());
            }
            return true;
        });

    // update
    traverse(
        instance->currentScene->getTopNode(), +[](Handle<Node> node) {
            node->update();
            return true;
        });
}

void SceneManager::setCurrentScene(Handle<Scene> scene)
{
    instance->currentScene = scene;
    instance->cameras.clear();
}

const std::vector<Handle<Camera>>& SceneManager::getCameras()
{
    return instance->cameras;
}

std::unordered_map<std::string, RenderQueue> SceneManager::buildRenderQueuesFor(RenderParameters& parameters)
{
    std::unordered_map<std::string, RenderQueue> queues;

    // Init queues
    for (auto& renderpass : RenderPassManager::getOrderedRenderPasses())
    {
        queues[renderpass->getName()] = RenderQueue();
    }

    // Fill queues with visible nodes
    traverse(instance->currentScene->getTopNode(), [&queues, &parameters](Handle<Node> node) {
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

        // tmp while no culling
        if (node->isA<PointLight>())
        {
            parameters.lights.pointLights.push_back(node->toHandle<PointLight>());
        }
        return true;
    });

    return queues;
}
