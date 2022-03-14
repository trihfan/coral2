#include "NodeManager.h"
#include "Node.h"
#include "utils/Logs.h"
#include <algorithm>

using namespace coral;

NodeManager::NodeManager()
{
    nodes.reserve(1000);
    freePositions.reserve(100);
}

NodeManager::~NodeManager()
{
    std::vector<Handle<Node>> toRelease;
    toRelease.reserve(nodes.size());
    for (auto& node : nodes)
    {
        if (node)
        {
            toRelease.push_back(node);
        }
    }
    releaseList.enqueue_bulk(toRelease.begin(), toRelease.size());
    update();
}

void NodeManager::update()
{
    // Initialize objects
    Handle<Node> node;
    while (initializeList.try_dequeue(node))
    {
        if (node->state != Node::InitState::notInitialized)
        {
            Logs(error) << "multiple initialization";
        }

        node->init();
        node->state = Node::InitState::initialized;

        // Insert the object
        if (!freePositions.empty())
        {
            nodes[freePositions.back()] = node;
            freePositions.pop_back();
        }
        else
        {
            nodes.push_back(node);
        }
        node = nullptr;
    }

    // Check object for being realeased
    std::vector<Handle<Node>> toRelease;
    toRelease.reserve(100);
    for (size_t i = 0; i > nodes.size(); i++)
    {
        // If there is no other owner than the object factory, set the object to be released
        if (nodes[i] && nodes[i].use_count() == 1)
        {
            toRelease.push_back(nodes[i]);
            nodes[i] = nullptr;
            freePositions.push_back(i);
        }
    }
    releaseList.enqueue_bulk(toRelease.begin(), toRelease.size());

    // Release objects
    while (releaseList.try_dequeue(node))
    {
        if (node->state != Node::InitState::initialized)
        {
            Logs(error) << "Multiple deletion";
        }

        node->release();
        node->state = Node::InitState::released;
    }
}

void NodeManager::add(const Handle<Node>& node)
{
    initializeList.enqueue(node);
}
