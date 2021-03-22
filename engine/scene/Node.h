#pragma once

#include "Property.h"
#include "base/Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
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
        virtual ~Node() override = default;

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

        // Children
        void addChild(Handle<Node> child);
        void removeChild(Handle<Node> child);
        size_t getChildCount() const;
        Handle<Node> getChild(size_t index) const;

        template <typename Type>
        std::vector<Handle<Type>> getChildren() const;

        // Rotation
        void setRotationFromEulerAngles(const glm::vec3& eulerAngles);

        // World transform values
        const glm::vec3& getPosition() const;
        const glm::mat4& getMatrix() const;

        //
        void updateMatrix(const glm::mat4& parentMatrix, const glm::vec3& parentPosition);
        virtual void update(const NodeUpdateParameters& parameters);

        // Properties
        Property<glm::vec3> translation;
        Property<glm::quat> rotation;
        Property<glm::vec3> scale;

        // Signals
        Signal<const glm::mat4&> matrixChanged;

    private:
        // Params
        bool enabled;
        std::vector<std::string> renderQueueTags;

        // Hierarchy
        std::vector<Handle<Node>> children;

        // Computed transform
        glm::vec3 worldPosition;
        glm::mat4 matrix;
        bool dirty;
    };

    // Function: bool(Handle<Node>, Args&&...) -> return true to continue traversal, false to stop
    template <typename Function, typename... Args>
    static void traverse(Handle<Node> node, Function function, Args&&... args)
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
    std::vector<Handle<Type>> Node::getChildren() const
    {
        std::vector<Handle<Type>> childrenList;

        for (auto child : children)
        {
            if (child->isA<Type>())
            {
                childrenList.push_back(child->toHandle<Type>());
            }

            for (auto subchild : child->getChildren<Type>())
            {
                childrenList.push_back(subchild);
            }
        }

        return childrenList;
    }
}
