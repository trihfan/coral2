#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include <utility>
#include "Object.h"

namespace coral
{
	// A node represent an item in the scene
	class Node : public Object
	{
		DECLARE_TYPE(Node)
	public:
		// construction
		Node(std::shared_ptr<Node> parent = nullptr);
		
		// Parent
		void setParent(std::shared_ptr<Node> parent);
		std::shared_ptr<Node> getParent() const;

		// Children
		void addChild(std::shared_ptr<Node> child);
		void removeChild(std::shared_ptr<Node> child);
		bool hasChild(std::shared_ptr<Node> child) const;
		size_t getChildrenCount() const;
		const std::vector<std::shared_ptr<Node>>& getChildren() const;

		// meta
		virtual bool isDrawable() const;

		// transform

		// uptate
		virtual void update() {}

	private:
		std::shared_ptr<Node> parent;
		std::vector<std::shared_ptr<Node>> children;
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
