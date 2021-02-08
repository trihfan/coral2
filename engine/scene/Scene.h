#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <set>
#include <unordered_map>
#include "Object.h"
#include "utils/Singleton.h"

namespace coral
{
	class Node;
	class DrawableNode;
	class Camera;
	class Material;
	class Shader;
	class Light;

	//
	class Scene : public Object
	{
	public:
		Scene();

		// Objects
		void add(std::shared_ptr<Node> node);
		void remove(std::shared_ptr<Node> node);

		// Get top node
		std::shared_ptr<Node> getTopNode() const;

	private:
		std::shared_ptr<Node> top_node;
	};

	//
	struct RenderQueue
	{
		std::vector<std::shared_ptr<DrawableNode>> nodes;
		std::unordered_map<std::shared_ptr<Shader>, std::set<std::shared_ptr<Material>>> shader_map;
		std::map<std::shared_ptr<Material>, std::vector<std::shared_ptr<DrawableNode>>> material_map;
	};

	// The scene manager
    class SceneManager
    {
        MAKE_ENGINE_SINGLETON(SceneManager)
    public:
		static void setCurrentScene(std::shared_ptr<Scene> scene);
		static void update();

    private:
		SceneManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);

    private:
		std::shared_ptr<Scene> current_scene;
		std::vector<std::shared_ptr<Camera>> cameras;
		std::vector<std::shared_ptr<Light>> lights;
		std::map<unsigned int, RenderQueue> render_queues;
    };
}
#endif
