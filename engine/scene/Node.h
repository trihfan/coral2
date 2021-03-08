#pragma once

#include "Object.h"
#include <glm/glm.hpp>
#include <memory>
#include <utility>
#include <vector>

namespace coral
{
    // A node represent an item in the scene
    class Node : public Object
    {
    public:
        // construction
        Node(Handle<Node> parent = nullptr);
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

        // Parent
        void setParent(Handle<Node> parent);
        Handle<Node> getParent() const;

        // Children
        void addChild(Handle<Node> child);
        void removeChild(Handle<Node> child);
        size_t getChildCount() const;
        Handle<Node> getChild(size_t index) const;

        // Translation
        void setTranslation(const glm::vec3& translation);
        const glm::vec3& getTranslation() const;

        // Rotation
        void setRotation(const glm::vec3& rotation);
        const glm::vec3& getRotation() const;

        // Scale
        void setScale(const glm::vec3& scale);
        const glm::vec3& getScale() const;

        // World transform values
        const glm::vec3& getPosition() const;
        const glm::mat4& getMatrix() const;

        //
        virtual void update() override;

        // Signals
        Signal<const glm::mat4&> matrixChanged;

    private:
        // Params
        bool enabled;
        std::vector<std::string> renderQueueTags;

        // Hierarchy
        Handle<Node> parent;
        std::vector<Handle<Node>> children;

        // Transform
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;

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
}
