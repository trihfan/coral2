#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

namespace coral
{
	class Object;
	class Node;

	class Scene
	{
	public:
		void add(std::shared_ptr<Node> node);
		void remove(std::shared_ptr<Node> node);
		const std::vector<std::shared_ptr<Node>>& allNodes() const;

	private:
		std::vector<std::shared_ptr<Node>> nodes;
		std::vector<std::shared_ptr<Object>> initializeList;
		std::vector<std::shared_ptr<Object>> releaseList;
	};
}
#endif
