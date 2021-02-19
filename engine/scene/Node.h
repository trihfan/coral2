#pragma once

#include "Object.h"
#include "Property.h"
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

        // meta
        virtual bool isDrawable() const;

        // Properties
        Property<bool> enabled;
        Property<Handle<Node>> parent;
        PropertyArray<Handle<Node>> children;
        Property<glm::vec3> position;
        Property<glm::vec3> rotation;
        Property<glm::vec3> scale;

        // Signals
        Signal<const glm::mat4&> matrixChanged;

        // World transform values
        const glm::vec3& getWorldPosition() const;
        const glm::vec3& getWorldRotation() const;
        const glm::vec3& getWorldScale() const;
        const glm::mat4& getWorldMatrix() const;

    private:
        glm::vec3 worldPosition;
        glm::vec3 worldRotation;
        glm::vec3 worldScale;
        glm::mat4 matrix;

        void updateWorldPosition(const glm::vec3& position);
        void updateWorldRotation(const glm::vec3& rotation);
        void updateWorldScale(const glm::vec3& scale);
        void udpateMatrix(const glm::vec3& parentPosition, const glm::vec3& parentRotation, const glm::vec3& parentScale);
    };

    // Function: bool(Handle<Node>, Args&&...) -> return true to continue traversal, false to stop
    template <typename Function, typename... Args>
    static void traverse(Handle<Node> node, Function function, Args&&... args)
    {
        if (*node->enabled && function(node, std::forward<Args>(args)...))
        {
            for (size_t i = 0; i < node->children.size(); i++)
            {
                traverse(node->children[i], std::forward<decltype(function)>(function), std::forward<Args>(args)...);
            }
        }
    }
}
