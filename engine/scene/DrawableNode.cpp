#include "DrawableNode.h"
#include "Engine.h"
#include "materials/Material.h"

using namespace coral;

DrawableNode::DrawableNode(Handle<Node> parent)
    : Node(parent)
{
}

void DrawableNode::setMaterial(Handle<Material> material)
{
    this->material = material;
}

Handle<Material> DrawableNode::getMaterial() const
{
    return material;
}

bool DrawableNode::isDrawable() const
{
    return true;
}

void DrawableNode::update()
{
    Node::update();
    if (material)
    {
        material->update();
    }
}
