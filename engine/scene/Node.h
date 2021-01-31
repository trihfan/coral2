#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include <utility>
#include <glm/glm.hpp>
#include "Object.h"
#include "Property.h"

namespace coral
{
	// A vec3 with local and world value
	struct TransformVec3
	{
		glm::vec3 local = glm::vec3(0, 0, 0);
		glm::vec3 world = glm::vec3(0, 0, 0);
	};

	// A node represent an item in the scene
	class Node : public Object
	{
	public:
		// construction
		Node(std::shared_ptr<Node> parent = nullptr);
		
		// Parent
		Property<std::shared_ptr<Node>> parentt;
		void setParent(std::shared_ptr<Node> parent);
		std::shared_ptr<Node> getParent() const;

		// Children
		void addChild(std::shared_ptr<Node> child);
		void removeChild(std::shared_ptr<Node> child);
		bool hasChild(std::shared_ptr<Node> child) const;
		size_t getChildrenCount() const;
		const std::vector<std::shared_ptr<Node>>& getChildren() const;

		// Position
		void setPosition(const glm::vec3& localPosition);
		const TransformVec3& getPosition() const;

		// Rotation
		void setRotation(const glm::vec3& localRotation);
		const TransformVec3& getRotation() const;

		// Scale
		void setScale(const glm::vec3& localScale);
		const TransformVec3& getScale() const;

		// Return transform matrix
		const glm::mat4& getMatrix() const;

		// meta
		virtual bool isDrawable() const;

		// uptate
		virtual void update() {}

		Signal<std::shared_ptr<Node>> parentChanged;

	private:
		// Hierarchy
		std::shared_ptr<Node> parent;
		std::vector<std::shared_ptr<Node>> children;
		
		// Transform
		std::shared_ptr<Node> object;
		TransformVec3 position;
		TransformVec3 rotation;
		TransformVec3 scale;
		glm::mat4 matrix;

		void udpateMatrix(const glm::vec3& parentPosition, const glm::vec3& parentRotation, const glm::vec3& parentScale);
	};

	// Function: bool(std::shared_ptr<Node>, Args&&...) -> return true to continue traversal, false to stop
	template <typename Function, typename... Args>
	static void traverse(std::shared_ptr<Node> node, Function function, Args&&... args)
	{
		if (function(node, std::forward<Args>(args)...))
		{
			for (std::shared_ptr<Node> child : node->getChildren())
			{
				traverse(child, std::forward<decltype(function)>(function), std::forward<Args>(args)...);
			}
		}
	}
}
#endif
