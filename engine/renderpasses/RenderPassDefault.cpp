#include "RenderPassDefault.h"
#include "Engine.h"
#include "backend/opengl/OpenglError.h"
#include "materials/Material.h"
#include "resources/Resource.h"
#include "resources/Shader.h"
#include "scene/DrawableNode.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"

using namespace coral;

void RenderPassDefault::internalRender(RenderQueue& queue, const RenderParameters& parameters)
{
    // setup rendering
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    CHECK_OPENGL_ERROR

    // for each shader
    for (auto& shader_pair : queue.shaderMap)
    {
        // Set up shader
        shader_pair.first->use();

        // for each material
        for (auto material : shader_pair.second)
        {
            // Set up material
            material->use(parameters);

            // draw each node
            for (auto node : queue.materialMap[material])
            {
                node->draw(parameters);
            }
        }
    }
}
