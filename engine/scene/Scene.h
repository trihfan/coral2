#ifndef SCENE_H
#define SCENE_H

#include "Object.h"

namespace coral
{
    class Node;

    //
    class Scene : public Object
    {
    public:
        Scene();

        // Objects
        void add(Handle<Node> node);
        void remove(Handle<Node> node);

        // Get top node
        Handle<Node> getTopNode() const;

    private:
        Handle<Node> top_node;
    };
}
#endif
