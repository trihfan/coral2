#pragma once
#include "utils/ConcurrentQueue.h"
#include "Handle.h"
#include <memory>
#include <string>
#include <vector>

namespace coral
{
    class Node;

    /**
     * @brief
     */
    class NodeManager
    {
    public:
        // Construction
        NodeManager();
        ~NodeManager();

        // Init and release nodes
        void update();

    private:
        // Init/release queues, thread safes
        moodycamel::ConcurrentQueue<Handle<Node>> initializeList;
        moodycamel::ConcurrentQueue<Handle<Node>> releaseList;
        moodycamel::ConcurrentQueue<Handle<Node>> resetList;

        // Array with all the nodes
        std::vector<Handle<Node>> nodes;
        std::vector<size_t> freePositions;

        // Add a new node
        friend class Node;
        void add(const Handle<Node>& node);
        void reset(const Handle<Node>& node);
    };
}
