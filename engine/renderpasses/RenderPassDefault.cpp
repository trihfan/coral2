#include "RenderPassDefault.h"
#include "BackendCommandBuffer.h"
#include "Engine.h"
#include "materials/Material.h"
#include "resources/Pipeline.h"
#include "resources/Resource.h"
#include "scene/DrawableNode.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/camera/Camera.h"

using namespace coral;

void RenderPassDefault::internalRender(RenderQueue& queue, const RenderParameters& parameters)
{
    auto commandBuffer = backend::BackendCommandBuffer::getCommandBuffer();

    // Camera viewport
    const auto& viewport = parameters.camera->getViewport();
    commandBuffer->setViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    // Clear buffers
    const glm::vec4 clearColor = parameters.camera->getBackgroundColor();
    commandBuffer->clearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    commandBuffer->clearDepth();

    // for each shader
    for (auto& pipelinePair : queue.pipelineMap)
    {
        // Set up shader
        pipelinePair.first->use();

        // for each material
        for (auto material : pipelinePair.second)
        {
            // Set up material
            material->use(parameters);

            // draw each node
            for (auto node : queue.materialMap[material])
            {
                material->setNode(node);
                node->draw(parameters);
            }
        }
    }
}
