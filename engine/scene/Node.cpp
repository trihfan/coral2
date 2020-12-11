#include "Node.h"

using namespace coral;

Node::Node(std::shared_ptr<Node> parent) :
	parent(parent)
{
}

void Node::setParent(std::shared_ptr<Node> parent)
{
	this->parent = parent;
}

std::shared_ptr<Node> Node::getParent() const
{
	return parent;
}

void Node::addChild(std::shared_ptr<Node> child)
{
	if (std::find(children.begin(), children.end(), child) == children.end())
	{
		children.push_back(child);
	}
	//child->parent = shared_from_this();
}

void Node::removeChild(std::shared_ptr<Node> child)
{
	auto it = std::find(children.begin(), children.end(), child);
	if (it != children.end())
	{
		children.erase(it);
	}
	child->parent = nullptr;
}

bool Node::hasChild(std::shared_ptr<Node> child) const
{
	return std::find(children.begin(), children.end(), child) != children.end();
}

size_t Node::getChildrenCount() const
{
	return children.size();
}

const std::vector<std::shared_ptr<Node>>& Node::getChildren() const
{
	return children;
}
