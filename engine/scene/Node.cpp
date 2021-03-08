#include "Node.h"
#include "renderpasses/RenderPassManager.h"
#include <algorithm>
#include <glm/gtx/transform.hpp>

using namespace coral;

Node::Node(Handle<Node> parent)
    : enabled(true)
    , translation(0, 0, 0)
    , rotation(0, 0, 0)
    , scale(1, 1, 1)
    , worldPosition(0, 0, 0)
    , matrix(1.f)
    , dirty(true)
{
    this->parent = parent;
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
}

void Node::setParent(Handle<Node> parent)
{
    this->parent = parent;
    parent->children.push_back(toHandle<Node>());
    dirty = true;
}

Handle<Node> Node::getParent() const
{
    return parent;
}

void Node::addChild(Handle<Node> child)
{
    children.push_back(child);
    child->parent = toHandle<Node>();
    child->dirty = true;
}

void Node::removeChild(Handle<Node> child)
{
    auto it = std::find_if(children.begin(), children.end(), [&child](const Handle<Node>& other) {
        return child == other;
    });
    if (it != children.end())
    {
        child->parent = nullptr;
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

void Node::setTranslation(const glm::vec3& translation)
{
    this->translation = translation;
    dirty = true;
}

const glm::vec3& Node::getTranslation() const
{
    return translation;
}

void Node::setRotation(const glm::vec3& rotation)
{
    this->rotation = rotation;
    dirty = true;
}

const glm::vec3& Node::getRotation() const
{
    return rotation;
}

void Node::setScale(const glm::vec3& scale)
{
    this->scale = scale;
    dirty = true;
}

const glm::vec3& Node::getScale() const
{
    return scale;
}

const glm::vec3& Node::getPosition() const
{
    return worldPosition;
}

const glm::mat4& Node::getMatrix() const
{
    return matrix;
}

void Node::update()
{
    Object::update();

    if (dirty)
    {
        worldPosition = getTranslation();
        if (parent)
        {
            worldPosition += parent->getPosition();
        }

        matrix = glm::mat4(1.f);
        matrix = glm::translate(matrix, getTranslation());
        matrix = glm::rotate(matrix, glm::radians(getRotation().x), glm::vec3(1.0, 0.0, 0.0));
        matrix = glm::rotate(matrix, glm::radians(getRotation().y), glm::vec3(0.0, 1.0, 0.0));
        matrix = glm::rotate(matrix, glm::radians(getRotation().z), glm::vec3(0.0, 0.0, 1.0));
        matrix = glm::scale(matrix, getScale());

        if (parent)
        {
            matrix = matrix * parent->getMatrix();
        }

        matrixChanged(matrix);
        dirty = false;
    }

    for (size_t i = 0; i < children.size(); i++)
    {
        children[i]->update();
    }
}
