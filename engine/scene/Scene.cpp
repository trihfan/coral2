#include "Scene.h"

using namespace coral;

void Scene::add(std::shared_ptr<Node> node)
{
	nodes.push_back(node);
}

void Scene::remove(std::shared_ptr<Node> node)
{
	auto it = std::find(nodes.begin(), nodes.end(), node);
	if (it != nodes.end())
	{
		nodes.erase(it);
	}
}

const std::vector<std::shared_ptr<Node>>& Scene::allNodes() const
{
	return nodes;
}