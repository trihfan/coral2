#include "Node.h"
#include <algorithm>
#include <glm/gtx/transform.hpp>

using namespace coral;

Node::Node(Handle<Node> parent)
{
    this->parent = parent;
    enabled = true;
    matrix = glm::mat4(1);
    connect<&Node::updateWorldPosition>(position.changed, this);
    connect<&Node::updateWorldRotation>(rotation.changed, this);
    connect<&Node::updateWorldScale>(scale.changed, this);
}

bool Node::isDrawable() const
{
    return false;
}

void Node::updateWorldPosition(const glm::vec3& position)
{
    const auto worldPosition = position + (*parent ? *parent->position : glm::vec3(0, 0, 0));
    udpateMatrix(worldPosition, worldRotation, worldScale);
}

void Node::updateWorldRotation(const glm::vec3& rotation)
{
    const auto worldRotation = rotation + (*parent ? *parent->rotation : glm::vec3(0, 0, 0));
    udpateMatrix(worldPosition, worldRotation, worldScale);
}

void Node::updateWorldScale(const glm::vec3& scale)
{
    const auto worldScale = scale + (*parent ? *parent->scale : glm::vec3(0, 0, 0));
    udpateMatrix(worldPosition, worldRotation, worldScale);
}

void Node::udpateMatrix(const glm::vec3& parentPosition, const glm::vec3& parentRotation, const glm::vec3& parentScale)
{
    worldPosition = parentPosition + *position;
    worldRotation = parentRotation + *rotation;
    worldScale = parentScale + *scale;

    matrix = glm::rotate(glm::mat4(1), worldRotation.x, glm::vec3(1, 0, 0));
    matrix = glm::rotate(matrix, worldRotation.y, glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, worldRotation.z, glm::vec3(0, 0, 1));
    matrix = glm::translate(matrix, worldPosition);
    matrix = glm::scale(matrix, worldScale);
    matrixChanged(matrix);

    for (size_t i = 0; i < children.size(); i++)
    {
        children[i]->udpateMatrix(worldPosition, worldRotation, worldScale);
    }
}

const glm::vec3& Node::getWorldPosition() const
{
    return worldPosition;
}

const glm::vec3& Node::getWorldRotation() const
{
    return worldRotation;
}

const glm::vec3& Node::getWorldScale() const
{
    return worldScale;
}

const glm::mat4& Node::getWorldMatrix() const
{
    return matrix;
}
