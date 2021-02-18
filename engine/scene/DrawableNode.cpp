#include "DrawableNode.h"
#include "Engine.h"
#include "materials/Material.h"

using namespace coral;

DrawableNode::DrawableNode(Handle<Node> parent, const std::vector<std::string>& renderQueueTags)
    : Node(parent)
{
    for (const auto& tag : renderQueueTags)
    {
        addRenderQueueTag(tag);
    }
}

void DrawableNode::setMaterial(Handle<Material> material)
{
    this->material = material;
}

Handle<Material> DrawableNode::getMaterial() const
{
    return material;
}

void DrawableNode::addRenderQueueTag(const std::string& renderQueueId)
{
    auto it = std::find(renderQueueTags.begin(), renderQueueTags.end(), renderQueueId);
    if (it == renderQueueTags.end())
    {
        renderQueueTags.push_back(renderQueueId);
    }
}

void DrawableNode::removeRenderQueueTag(const std::string& renderQueueId)
{
    auto it = std::find(renderQueueTags.begin(), renderQueueTags.end(), renderQueueId);
    if (it != renderQueueTags.end())
    {
        renderQueueTags.erase(it);
    }
}

bool DrawableNode::isTagForRenderQueue(const std::string& renderQueueId) const
{
    return std::find(renderQueueTags.begin(), renderQueueTags.end(), renderQueueId) != renderQueueTags.end();
}

const std::vector<std::string>& DrawableNode::getRenderQueueTags() const
{
    return renderQueueTags;
}

bool DrawableNode::isDrawable() const
{
    return true;
}
