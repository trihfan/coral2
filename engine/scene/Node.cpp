#include "Node.h"
#include <algorithm>
#include <glm/gtx/transform.hpp>

using namespace coral;

Node::Node(std::shared_ptr<Node> parent) : parent(parent)
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
	child->parent = toHandle<Node>();
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

bool Node::isDrawable() const
{
	return false;
}

void Node::setPosition(const glm::vec3& localPosition)
{
	position.local = localPosition;
	udpateMatrix(localPosition + (getParent() ? getParent()->getPosition().local : glm::vec3(0, 0, 0)), rotation.world, scale.world);
}

const TransformVec3& Node::getPosition() const
{
	return position;
}

void Node::setRotation(const glm::vec3& localRotation)
{
	rotation.local = localRotation;
	udpateMatrix(position.local, localRotation + (getParent() ? getParent()->getRotation().local : glm::vec3(0, 0, 0)), scale.world);
}

const TransformVec3& Node::getRotation() const
{
	return rotation;
}

void Node::setScale(const glm::vec3& localScale)
{
	scale.local = localScale;
	udpateMatrix(position.local, rotation.local, localScale + (getParent() ? getParent()->getRotation().local : glm::vec3(0, 0, 0)));
}

const TransformVec3& Node::getScale() const
{
	return scale;
}

const glm::mat4& Node::getMatrix() const
{
	return matrix;
}

void Node::udpateMatrix(const glm::vec3& parentPosition, const glm::vec3& parentRotation, const glm::vec3& parentScale)
{
	position.world = parentPosition + position.local;
	rotation.world = parentRotation + rotation.local;
	scale.world = parentScale + scale.local;

	matrix = glm::rotate(matrix, rotation.world.x, glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, rotation.world.y, glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, rotation.world.z, glm::vec3(0, 0, 1));
	matrix = glm::translate(glm::mat4(), position.world);
	matrix = glm::scale(matrix, scale.world);
	
	for (auto child : getChildren())
	{
		child->udpateMatrix(position.world, rotation.world, scale.world);
	}
}