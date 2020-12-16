#include <functional>
#include "Scene.h"
#include "Node.h"
#include "Camera.h"

using namespace coral;

//*********************************************************************************
// Scene

void Scene::add(std::shared_ptr<Node> node)
{
	top_node->addChild(node);
}

void Scene::remove(std::shared_ptr<Node> node)
{
	top_node->removeChild(node);
}

//*********************************************************************************
// SceneManager

DEFINE_SINGLETON(SceneManager)

static void fillLists(std::shared_ptr<Node> node)
{
	for (std::shared_ptr<Node> child : node->getChildren())
	{
		fillLists(child);
	}
}

void SceneManager::update()
{
	if (!instance->current_scene)
	{
		return;
	}

	// input and list camera for render
	instance->cameras.clear();
	traverse(instance->current_scene->top_node, [](std::shared_ptr<Node> node)
	{
		// send input
		// todo

		if (node->isA(Camera::type))
		{
			instance->cameras.push_back(node->getHandle<Camera>());
		}
		return true;
	});

	// update
	traverse(instance->current_scene->top_node, [](std::shared_ptr<Node> node)
	{ 
		node->update(); 
		if (node->isA(Camera::type))
		{
			instance->cameras.push_back(node->getHandle<Camera>());
		}
		return true; 
	});

	// cull / fill render queues
	traverse(instance->current_scene->top_node, [](std::shared_ptr<Node> node)
	{
		node->update();
		if (node->isA(Camera::type))
		{
			instance->cameras.push_back(node->getHandle<Camera>());
		}
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
	// add default queue
	render_queues.insert(std::make_pair(1000, RenderQueue()));
}