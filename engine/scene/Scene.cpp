#include "Scene.h"
#include "Node.h"
#include "ObjectFactory.h"

using namespace coral;

Scene::Scene()
    : top_node(ObjectFactory::create<Node>())
{
}

void Scene::add(Handle<Node> node)
{
    top_node->addChild(node);
}

void Scene::remove(Handle<Node> node)
{
    top_node->removeChild(node);
}

Handle<Node> Scene::getTopNode() const
{
    return top_node;
}
