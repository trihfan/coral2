#pragma once
#include "Property.h"
#include "Handle.h"

namespace coral
{
    class Node;

    //
    class Scene
    {
    public:
        Scene();

        // Objects
        void add(Handle<Node> node);
        void remove(Handle<Node> node);

        // Properties
        Property<Handle<Node>> topNode;
    };
}
