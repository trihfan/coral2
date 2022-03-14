#include "SceneManager.h"
#include "DrawableNode.h"
#include "Node.h"
#include "RenderParameters.h"
#include "Scene.h"
#include "DrawableNode.h"
#include "camera/Camera.h"
#include "light/PointLight.h"
#include "materials/Material.h"
#include "renderpasses/RenderPass.h"
#include <functional>

using namespace coral;

void SceneManager::update()
{
    // Check scene
    if (!currentScene)
    {
        return;
    }

    // List objects of interests
    cameras.clear();
    lights.clear();
    traverse(currentScene->topNode, [this](Handle<Node> node)
    {
        if (node->isA<Camera>())
        {
            cameras.push_back(node->toPtr<Camera>());
        }
        return true;
    });

    // Update nodes
    currentScene->topNode->update();
}

void SceneManager::setCurrentScene(Handle<Scene> scene)
{
    currentScene = scene;
    cameras.clear();
}

const std::vector<Handle<Camera>>& SceneManager::getCameras()
{
    return cameras;
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
    traverse(currentScene->topNode, [&queues, &parameters](Handle<Node> node) {
        if (node->isA<DrawbleNode>())
        {
            auto drawableNode = node->toHandle<DrawableNode>();
            for (const auto& id : drawableNode->getRenderQueueTags())
            {
                assert(drawableNode->getMaterial()->getPipeline());
                auto& render_queue = queues[id];
                render_queue.nodes.push_back(drawableNode);
                render_queue.pipelineMap[drawableNode->getMaterial()->getPipeline()].insert(drawableNode->getMaterial());
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
