#pragma once
#include "Object.h"
#include <map>
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
    class Pipeline;
    class Light;
    struct RenderParameters;

    /**
     * @brief A render queue, contains a list of nodes to be render in a specific renderpass
     */
    struct RenderQueue
    {
        std::vector<std::shared_ptr<DrawableNode>> nodes;
        std::map<std::shared_ptr<Pipeline>, std::set<std::shared_ptr<Material>>> pipelineMap;
        std::map<std::shared_ptr<Material>, std::vector<std::shared_ptr<DrawableNode>>> materialMap;
    };

    /**
     * @brief The SceneManager class contains the current scene and is responsible for building the render queues
     */
    class SceneManager
    {
    public:
        /**
         * @brief Set the current scene
         */
        void setCurrentScene(const std::shared_ptr<Scene>& scene);

        /**
         * @brief Return the cameras
         */
        const std::vector<std::shared_ptr<Camera>>& getCameras();

        /**
         * @brief Build the render queues for the given parameters
         */
        std::unordered_map<std::string, RenderQueue> buildRenderQueuesFor(RenderParameters& parameters);

        /**
         * @brief Update all nodes of the scene
         */
        void update();

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
