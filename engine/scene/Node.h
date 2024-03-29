#pragma once
#include "Property.h"
#include "PropertyArray.h"
#include "Transform.h"
#include "Object.h"
#include "ObjectInterface.h"
#include <utility>

namespace coral
{
    class Node;
    template <typename Function, typename... Args>
    static void traverse(Object<Node> node, Function function, Args&&... args);

    /**
     * @brief The Node class represent an item in the scene
     */
    class Node : public ObjectInterface, public Transform
    {
    public:
        // Construction
        Node();
        ~Node();

        // Properties
        Property<bool> enabled;
        PropertyArray<std::string> renderTags;
        Property<ObjectRef<Node>> parent;
        PropertyArray<Object<Node>> children;

        // Update the node, called each frame
        virtual void update();
    };

    // Function: bool(Handle<Node>, Args&&...) -> return true to continue traversal, false to stop
    template <typename Function, typename... Args>
    static void traverse(Object<Node> node, Function function, Args&&... args)
    {
        if (node->enabled && function(node, std::forward<Args>(args)...))
        {
            for (auto& child : node->children)
            {
                traverse(child, std::forward<decltype(function)>(function), std::forward<Args>(args)...);
            }
        }
    }
}
