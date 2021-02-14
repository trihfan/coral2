#ifndef RENDERPARAMETERS_H
#define RENDERPARAMETERS_H

#include <chrono>
#include <memory>
#include <vector>

namespace coral
{
    class Scene;
    class Camera;
    class Node;
    class RenderPass;
    class PointLight;
    class DirectionalLight;
    class SpotLight;
    class AreaLight;

    struct LightArray
    {
        std::vector<std::shared_ptr<PointLight>> pointLights;
        std::vector<std::shared_ptr<DirectionalLight>> directionalLights;
        std::vector<std::shared_ptr<SpotLight>> spotLights;
        std::vector<std::shared_ptr<AreaLight>> areaLights;
    };

    /**
     * @brief Parameters for the rendering
     */
    struct RenderParameters
    {
        // Scene
        std::shared_ptr<Camera> camera;
        LightArray lights;

        // Time
        double time;
        double deltaTime;

        // Viewport
        int width;
        int height;
    };
}
#endif
