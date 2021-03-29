#pragma once

#include "Property.h"
#include "Transform.h"
#include "base/Object.h"
#include <memory>
#include <utility>
#include <vector>

namespace coral
{
    struct NodeUpdateParameters
    {
        double time; // Time since engine start in seconds
        double deltaTime; // Time of the last frame in seconds
    };

    // A node represent an item in the scene
    class Node : public Object
    {
    public:
        // construction
        Node();
        virtual ~Node() override;

        // meta
        virtual bool isDrawable() const;

        // Render queue
        void addRenderQueueTag(const std::string& renderQueueId);
        void removeRenderQueueTag(const std::string& renderQueueId);
        bool isTagForRenderQueue(const std::string& renderQueueId) const;
        const std::vector<std::string>& getRenderQueueTags() const;

        // Enabled
        bool isEnabled() const;
        void setEnabled(bool enabled);

        // Transform
        const Transform& transform() const;
        Transform& transform();

        void setTranslation(const glm::vec3& translation);
        const glm::vec3& getTranslation() const;

        void setRotation(const glm::quat& rotation);
        void setRotation(const glm::vec3& rotationEuler);
        const glm::quat& getRotation() const;

        void setScale(const glm::vec3& scale);
        const glm::vec3& getScale() const;

        const glm::vec3& getWorldPosition() const;

        // Parent
        void setParent(Node* parent);
        Node* getParent() const;

        // Children
        void addChild(ptr<Node> child);
        void removeChild(ptr<Node> child);
        size_t getChildCount() const;
        ptr<Node> getChild(size_t index) const;
        template <typename Type>
        std::vector<ptr<Type>> getChildren() const;

        // Update the node, called each frame
        virtual void update(const NodeUpdateParameters& parameters);

    private:
        // Params
        bool enabled;
        std::vector<std::string> renderQueueTags;

        // Hierarchy
        Node* parent;
        std::vector<ptr<Node>> children;

        // Transform
        Transform nodeTransform;
    };

    // Function: bool(ptr<Node>, Args&&...) -> return true to continue traversal, false to stop
    template <typename Function, typename... Args>
    static void traverse(ptr<Node> node, Function function, Args&&... args)
    {
        if (node->isEnabled() && function(node, std::forward<Args>(args)...))
        {
            for (size_t i = 0; i < node->getChildCount(); i++)
            {
                traverse(node->getChild(i), std::forward<decltype(function)>(function), std::forward<Args>(args)...);
            }
        }
    }

    template <typename Type>
    std::vector<ptr<Type>> Node::getChildren() const
    {
        std::vector<ptr<Type>> childrenList;

        for (auto child : children)
        {
            if (child->isA<Type>())
            {
                childrenList.push_back(child->toPtr<Type>());
            }

            for (auto subchild : child->getChildren<Type>())
            {
                childrenList.push_back(subchild);
            }
        }

        return childrenList;
    }
}
