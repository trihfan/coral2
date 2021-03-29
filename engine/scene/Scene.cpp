#include "Scene.h"
#include "Node.h"
#include "base/ObjectFactory.h"

using namespace coral;

Scene::Scene()
    : top_node(ObjectFactory::create<Node>())
{
}

void Scene::add(ptr<Node> node)
{
    top_node->addChild(node);
}

void Scene::remove(ptr<Node> node)
{
    top_node->removeChild(node);
}

ptr<Node> Scene::getTopNode() const
{
    return top_node;
}
