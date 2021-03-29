#include "Node.h"
#include "renderpasses/RenderPassManager.h"
#include <algorithm>

using namespace coral;

Node::Node()
    : parent(nullptr)
    , enabled(true)
{
}

Node::~Node()
{
    for (auto child : children)
    {
        child->setParent(nullptr);
    }
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

void Node::setParent(Node* parent)
{
    this->parent = parent;
    nodeTransform.setDirty();
}

Node* Node::getParent() const
{
    return parent;
}

void Node::addChild(ptr<Node> child)
{
    children.push_back(child);
    child->setParent(this);
}

void Node::removeChild(ptr<Node> child)
{
    auto it = std::find_if(children.begin(), children.end(), [&child](const ptr<Node>& other) {
        return child == other;
    });
    if (it != children.end())
    {
        child->setParent(nullptr);
        children.erase(it);
    }
}

size_t Node::getChildCount() const
{
    return children.size();
}

ptr<Node> Node::getChild(size_t index) const
{
    return children[index];
}

void Node::update(const NodeUpdateParameters& parameters)
{
    nodeTransform.update(parent ? &parent->transform() : nullptr);

    for (size_t i = 0; i < children.size(); i++)
    {
        if (children[i]->isEnabled())
        {
            children[i]->update(parameters);
        }
    }
}

const Transform& Node::transform() const
{
    return nodeTransform;
}

Transform& Node::transform()
{
    return nodeTransform;
}

void Node::setTranslation(const glm::vec3& translation)
{
    nodeTransform.translation.set(translation);
}

const glm::vec3& Node::getTranslation() const
{
    return nodeTransform.translation.get();
}

void Node::setRotation(const glm::quat& rotation)
{
    nodeTransform.rotation.set(rotation);
}

void Node::setRotation(const glm::vec3& rotationEuler)
{
    nodeTransform.setRotationFromEulerAngles(rotationEuler);
}

const glm::quat& Node::getRotation() const
{
    return nodeTransform.rotation.get();
}

void Node::setScale(const glm::vec3& scale)
{
    nodeTransform.scale.set(scale);
}

const glm::vec3& Node::getScale() const
{
    return nodeTransform.scale.get();
}

const glm::vec3& Node::getWorldPosition() const
{
    return nodeTransform.getPosition();
}