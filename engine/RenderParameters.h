#pragma once
#include "Object.h"
#include "scene/light/AreaLight.h"
#include "scene/light/DirectionalLight.h"
#include "scene/light/PointLight.h"
#include "scene/light/SpotLight.h"
#include <chrono>
#include <memory>
#include <vector>

namespace coral
{
    class Scene;
    class Camera;
    class Node;
    class RenderPass;

    struct LightArray
    {
        std::vector<Object<PointLight>> pointLights;
        std::vector<Object<DirectionalLight>> directionalLights;
        std::vector<Object<SpotLight>> spotLights;
        std::vector<Object<AreaLight>> areaLights;
    };

    /**
     * @brief Parameters for the rendering
     */
    struct RenderParameters
    {
        // Scene
        Object<Camera> camera;
        LightArray lights;

        // Time (in seconds)
        double time;
        double deltaTime;

        // Viewport
        int width;
        int height;
    };
}
