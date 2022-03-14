#include "Node.h"
//#include "renderpasses/RenderPassManager.h"
#include "utils/Logs.h"
#include <algorithm>

using namespace coral;

Node::Node() :
    state(InitState::notInitialized),
    enabled(true)
{
    connect<&Transform::setDirty>(parent.modified, this);
}

Node::~Node()
{
    // Remove this node from its parent
    auto parent = this->parent->lock();
    if (parent)
    {
        for (auto& child : parent->children)
        {
            if (child == this)
            {
                parent->children.remove(child);
                break;
            }
        }
    }

    // Clear children list
    for (auto& child : children)
    {
        child->parent = nullptr;
    }

    if (state == InitState::initialized)
    {
        Logs(warning) << "object '" << name.get() << "' (" << this << ") has not been released";
    }
}


void Node::update()
{
    Transform::update(parent->lock());

    // to task
    for (Handle<Node>& child : children)
    {
        if (child->enabled)
        {
            child->update();
        }
    }
}
