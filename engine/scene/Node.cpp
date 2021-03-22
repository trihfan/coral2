#include "Node.h"
#include "renderpasses/RenderPassManager.h"
#include <algorithm>

using namespace coral;

Node::Node()
    : translation(glm::vec3(0, 0, 0))
    , scale(glm::vec3(1, 1, 1))
    , enabled(true)
    , matrix(1)
    , dirty(true)
{
    setRotationFromEulerAngles(glm::vec3(0, 0, 0));
    connect(translation.modified, [this]() { dirty = true; });
    connect(rotation.modified, [this]() { dirty = true; });
    connect(scale.modified, [this]() { dirty = true; });
}

bool Node::isDrawable() const
{
    return false;
}

bool Node::isEnabled() const
{
    return enabled;
}

void Node::addRenderQueueTag(const std::string& renderQueueId)
{
    auto it = std::find(renderQueueTags.begin(), renderQueueTags.end(), renderQueueId);
    if (it == renderQueueTags.end())
    {
        renderQueueTags.push_back(renderQueueId);
    }
}

void Node::removeRenderQueueTag(const std::string& renderQueueId)
{
    auto it = std::find(renderQueueTags.begin(), renderQueueTags.end(), renderQueueId);
    if (it != renderQueueTags.end())
    {
        renderQueueTags.erase(it);
    }
}

bool Node::isTagForRenderQueue(const std::string& renderQueueId) const
{
    return std::find(renderQueueTags.begin(), renderQueueTags.end(), renderQueueId) != renderQueueTags.end();
}

const std::vector<std::string>& Node::getRenderQueueTags() const
{
    return renderQueueTags;
}

void Node::setEnabled(bool enabled)
{
    this->enabled = enabled;

    for (auto child : children)
    {
        child->setEnabled(enabled);
    }
}

void Node::addChild(Handle<Node> child)
{
    children.push_back(child);
    child->dirty = true;
}

void Node::removeChild(Handle<Node> child)
{
    auto it = std::find_if(children.begin(), children.end(), [&child](const Handle<Node>& other) {
        return child == other;
    });
    if (it != children.end())
    {
        child->dirty = true;
        children.erase(it);
    }
}

size_t Node::getChildCount() const
{
    return children.size();
}

Handle<Node> Node::getChild(size_t index) const
{
    return children[index];
}

void Node::setRotationFromEulerAngles(const glm::vec3& eulerAngles)
{
    glm::quat quaternionX = glm::angleAxis(glm::radians(eulerAngles.x), glm::vec3(1.0, 0.0, 0.0));
    glm::quat quaternionY = glm::angleAxis(glm::radians(eulerAngles.y), glm::vec3(0.0, 1.0, 0.0));
    glm::quat quaternionZ = glm::angleAxis(glm::radians(eulerAngles.z), glm::vec3(0.0, 0.0, 1.0));
    glm::quat finalOrientation = quaternionX * quaternionY * quaternionZ;
    rotation.set(finalOrientation);
}

const glm::vec3& Node::getPosition() const
{
    return worldPosition;
}

const glm::mat4& Node::getMatrix() const
{
    return matrix;
}

void Node::updateMatrix(const glm::mat4& parentMatrix, const glm::vec3& parentPosition)
{
    if (dirty)
    {
        matrix = glm::translate(glm::mat4(1), translation.get());
        matrix *= glm::toMat4(rotation.get());
        matrix *= glm::scale(glm::mat4(1), scale.get());
        matrix = matrix * parentMatrix;
        worldPosition = translation.get() + parentPosition;
        dirty = false;
        matrixChanged(matrix);
    }

    for (size_t i = 0; i < children.size(); i++)
    {
        children[i]->updateMatrix(matrix, getPosition());
    }
}

void Node::update(const NodeUpdateParameters& parameters)
{
    for (size_t i = 0; i < children.size(); i++)
    {
        if (children[i]->isEnabled())
        {
            children[i]->update(parameters);
        }
    }
}
