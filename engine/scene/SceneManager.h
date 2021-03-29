#pragma once

#include "base/Object.h"
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
    class Pipeline;
    class Light;
    struct RenderParameters;

    /**
     * @brief A render queue, contains a list of nodes to be render in a specific renderpass
     */
    struct RenderQueue
    {
        std::vector<ptr<DrawableNode>> nodes;
        std::map<ptr<Pipeline>, std::set<ptr<Material>>> pipelineMap;
        std::map<ptr<Material>, std::vector<ptr<DrawableNode>>> materialMap;
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
        static void setCurrentScene(ptr<Scene> scene);

        /**
         * @brief Return the cameras
         */
        static const std::vector<ptr<Camera>>& getCameras();

        /**
         * @brief Update all nodes of the scene
         */
        static void update(const RenderParameters& parameters);

        /**
         * @brief Build the render queues for the given parameters
         */
        static std::unordered_map<std::string, RenderQueue> buildRenderQueuesFor(RenderParameters& parameters);

    private:
        SceneManager() = default;

        /**
         * @brief The current scene
         */
        ptr<Scene> currentScene;

        /**
         * @brief List of camera in the currrent scene
         */
        std::vector<ptr<Camera>> cameras;

        /**
         * @brief List of lights in the currrent scene
         */
        std::vector<ptr<Light>> lights;
    };
}
