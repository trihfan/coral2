#include "DrawableNode.h"
#include "Engine.h"
#include "materials/Material.h"

using namespace coral;

void DrawableNode::setMaterial(ptr<Material> material)
{
    this->material = material;
}

ptr<Material> DrawableNode::getMaterial() const
{
    return material;
}

bool DrawableNode::isDrawable() const
{
    return true;
}

void DrawableNode::update(const NodeUpdateParameters& parameters)
{
    Node::update(parameters);
    if (!material->getPipeline() || material->getPipeline()->isDirty())
    {
        material->invalidatePipeline();
    }
}
