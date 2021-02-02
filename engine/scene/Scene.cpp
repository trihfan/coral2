#include <functional>
#include "Scene.h"
#include "Node.h"
#include "camera/Camera.h"
#include "light/Light.h"

using namespace coral;

//*********************************************************************************
// Scene

Scene::Scene() :
	top_node(ObjectManager::create<Node>())
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

//*********************************************************************************
// SceneManager

DEFINE_SINGLETON(SceneManager)

void SceneManager::update()
{
	// check scene
	if (!instance->current_scene)
	{
		return;
	}

	// list cameras and lights
	instance->cameras.clear();
	instance->lights.clear();

	traverse(instance->current_scene->getTopNode(), [](std::shared_ptr<Node> node)
	{
		if (node->isA<Camera>())
		{
			instance->cameras.push_back(node->toHandle<Camera>());
		}
		else if (node->isA<Light>())
		{
			instance->lights.push_back(node->toHandle<Light>());
		}
		return true;
	});

	// update
	traverse(instance->current_scene->getTopNode(), [](std::shared_ptr<Node> node)
	{ 
		node->update(); 
		return true; 
	});
}

void SceneManager::setCurrentScene(std::shared_ptr<Scene> scene)
{
	instance->current_scene = scene;
	instance->cameras.clear();
}

SceneManager::SceneManager(std::pmr::memory_resource* memory_resource)
{
}