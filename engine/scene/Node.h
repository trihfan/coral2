#pragma once
#include "Property.h"
#include "PropertyArray.h"
#include "Transform.h"
#include "NodeManager.h"
#include "Engine.h"
#include <memory>
#include <utility>
#include <string>

namespace coral
{
    // A node represent an item in the scene
    class Node : public std::enable_shared_from_this<Node>, public Transform
    {
        friend class NodeManager;
    public:
        // Creation
        template <typename NodeType, class... Args> static Handle<NodeType> createWithName(const std::string& name, Args&&... args);
        template <typename NodeType, class... Args> static Handle<NodeType> create(Args&&... args);
        virtual ~Node();

        // meta
        template <typename Type> Handle<Type> toPtr();
        template <typename Type> bool isA() const;

        // Properties
        Property<std::string> name;
        Property<bool> enabled;
        PropertyArray<std::string> renderTags;
        Property<WeakHandle<Node>> parent;
        PropertyArray<Handle<Node>> children;

        // Update the node, called each frame
        virtual void update();

    protected:
        // Initialization methods
        virtual void init() {}
        virtual void release() {}

    private:
        // Meta
        enum class InitState { notInitialized, initialized, released };
        InitState state;

        // Construction
        Node();
    };

    // Function: bool(Handle<Node>, Args&&...) -> return true to continue traversal, false to stop
    template <typename Function, typename... Args>
    static void traverse(Handle<Node> node, Function function, Args&&... args)
    {
        if (node->enabled && function(node, std::forward<Args>(args)...))
        {
            for (auto& child : node->children)
            {
                traverse(child, std::forward<decltype(function)>(function), std::forward<Args>(args)...);
            }
        }
    }

    // -----------------------------------------------
    template <typename NodeType, class... Args>
    Handle<NodeType> Node::createWithName(const std::string& name, Args&&... args)
    {
        auto node = create<NodeType>(std::forward<Args>(args)...);
        node->setName(name);
        return node;
    }

    template <typename NodeType, class... Args>
    Handle<NodeType> Node::create(Args&&... args)
    {
        static_assert (std::is_base_of<Node, NodeType>::value, "can only create Node objects");
        auto node = std::make_shared<NodeType>(std::forward<Args>(args)...);
        engine->nodeManager->add(node);
        return node;
    }
    template <typename Type>
    Handle<Type> Node::toPtr()
    {
        return std::dynamic_pointer_cast<Type>(shared_from_this());
    }

    template <typename Type>
    bool Node::isA() const
    {
        return dynamic_cast<const Type*>(this) != nullptr;
    }
}
