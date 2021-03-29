#pragma once

#include "base/Object.h"

namespace coral
{
    class Node;

    //
    class Scene : public Object
    {
    public:
        Scene();

        // Objects
        void add(ptr<Node> node);
        void remove(ptr<Node> node);

        // Get top node
        ptr<Node> getTopNode() const;

    private:
        ptr<Node> top_node;
    };
}
