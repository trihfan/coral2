#include "DrawableNode.h"
#include "Engine.h"

using namespace coral;

DrawableNode::DrawableNode(std::shared_ptr<Node> parent) :
	Node(parent), render_queue(DefaultRenderPassId::defaultRenderPass)
{
}

void DrawableNode::setMaterial(std::shared_ptr<Material> material)
{
    this->material = material;
}

std::shared_ptr<Material> DrawableNode::getMaterial() const
{
    return material;
}

void DrawableNode::setRenderQueue(unsigned int render_queue)
{
	this->render_queue = render_queue;
}

unsigned int DrawableNode::getRenderQueue() const
{
	return render_queue;
}

bool DrawableNode::isDrawable() const
{
	return true;
}