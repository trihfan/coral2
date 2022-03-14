#include "DrawableNode.h"
#include "Engine.h"
#include "materials/Material.h"

using namespace coral;

void DrawableNode::update()
{
    Node::update();

    // Update pipeline
    if (!material->getPipeline() || material->getPipeline()->isDirty())
    {
        material->invalidatePipeline();
    }
}
