#pragma once
#include "Property.h"
#include "Object.h"

namespace coral
{
    class Node;

    //
    class Scene
    {
    public:
        Scene();

        // Objects
        void add(Object<Node> node);
        void remove(Object<Node> node);

        // Properties
        Property<Object<Node>> topNode;
    };
}
