#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>

namespace coral
{
	class Camera;

	class Node
	{
	public:
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

		// Draw
		virtual void draw(std::shared_ptr<coral::Camera> camera) 
		{
			for (auto child : children)
			{
				child->draw(camera);
			}
		}

	private:
		std::shared_ptr<Node> parent;
		std::vector<std::shared_ptr<Node>> children;
	};
}
#endif
