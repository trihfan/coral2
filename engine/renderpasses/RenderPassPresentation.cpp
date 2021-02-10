#include "RenderPassDefault.h"
#include "scene/Scene.h"
#include "scene/DrawableNode.h"
#include "materials/Material.h"
#include "Shader.h"
#include "Engine.h"

using namespace coral;

RenderPassDefault::RenderPassDefault()
{
    addOutput(RenderPassResource{ "backbuffer" });
}

void RenderPassDefault::internalRender(RenderQueue& queue)
{
    // for each shader
    for (auto& shader_pair : queue.shader_map)
    {
        // Set up shader
        shader_pair.first->use();

        // for each material
        for (auto material : shader_pair.second)
        {
            // Set up material
            material->use(Engine::current_parameters);

            // draw each node
            for (auto node : queue.material_map[material])
            {
                node->draw(Engine::current_parameters);
            }
        }
    }
}