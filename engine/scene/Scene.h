#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <functional>
#include <map>
#include "Object.h"
#include "utils/Singleton.h"

namespace coral
{
	class Node;
	class Camera;

	//
	class Scene : public Object
	{
		friend class SceneManager;
		DECLARE_TYPE(Scene)
	public:
		void add(std::shared_ptr<Node> node);
		void remove(std::shared_ptr<Node> node);

	private:
		std::shared_ptr<Node> top_node;
	};

	//
	struct RenderQueue
	{
		std::vector<std::shared_ptr<Node>> nodes;
	};

	// The scene manager
    class SceneManager
    {
        MAKE_ENGINE_SINGLETON(SceneManager)
    public:
		static void setCurrentScene(std::shared_ptr<Scene> scene);
		static void update();

    private:
		SceneManager(std::pmr::memory_resource* memory_resource);

    private:
		std::shared_ptr<Scene> current_scene;
		std::vector<std::shared_ptr<Camera>> cameras;
		std::map<unsigned int, RenderQueue> render_queues;
    };
}
#endif
