#include "Scene.h"
#include "Node.h"

using namespace coral;

Scene::Scene()
    : top_node(ObjectManager::create<Node>())
{
}

void Scene::add(std::shared_ptr<Node> node)
{
    top_node->children.add(node);
}

void Scene::remove(std::shared_ptr<Node> node)
{
    top_node->children.remove(node);
}

std::shared_ptr<Node> Scene::getTopNode() const
{
    return top_node;
}