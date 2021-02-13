#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Object.h"
#include "memory_resource.h"
#include "utils/Singleton.h"
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

namespace coral
{
    class Scene;
    class Node;
    class DrawableNode;
    class Camera;
    class Material;
    class Shader;
    class Light;
    struct RenderParameters;

    /**
     * @brief A render queue, contains a list of nodes to be render in a specific renderpass
     */
    struct RenderQueue
    {
        std::vector<std::shared_ptr<DrawableNode>> nodes;
        std::unordered_map<std::shared_ptr<Shader>, std::set<std::shared_ptr<Material>>> shaderMap;
        std::map<std::shared_ptr<Material>, std::vector<std::shared_ptr<DrawableNode>>> materialMap;
    };

    /**
     * @brief The SceneManager class contains the current scene and is responsible for building the render queues
     */
    class SceneManager
    {
        MAKE_SINGLETON(SceneManager)
    public:
        /**
         * @brief Set the current scene
         */
        static void setCurrentScene(std::shared_ptr<Scene> scene);

        /**
         * @brief Return the cameras
         */
        static const std::vector<std::shared_ptr<Camera>>& getCameras();

        /**
         * @brief Update all nodes of the scene
         */
        static void update(const RenderParameters& parameters);

        /**
         * @brief Build the render queues for the given parameters
         */
        static std::unordered_map<std::string, RenderQueue> buildRenderQueuesFor(const RenderParameters& parameters);

    private:
        // Constructor
        SceneManager(std::shared_ptr<std::pmr::memory_resource> memoryResource);

    private:
        /**
         * @brief The current scene
         */
        std::shared_ptr<Scene> currentScene;

        /**
         * @brief List of camera in the currrent scene
         */
        std::vector<std::shared_ptr<Camera>> cameras;

        /**
         * @brief List of lights in the currrent scene
         */
        std::vector<std::shared_ptr<Light>> lights;
    };
}
#endif
