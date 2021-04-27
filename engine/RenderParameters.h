#pragma once

#include "base/Ptr.h"
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
        std::vector<ptr<PointLight>> pointLights;
        std::vector<ptr<DirectionalLight>> directionalLights;
        std::vector<ptr<SpotLight>> spotLights;
        std::vector<ptr<AreaLight>> areaLights;
    };

    /**
     * @brief Parameters for the rendering
     */
    struct RenderParameters
    {
        // Scene
        ptr<Camera> camera;
        LightArray lights;

        // Time (in seconds)
        double time;
        double deltaTime;

        // Viewport
        int width;
        int height;

        void clear()
        {
            camera = nullptr;
            lights = LightArray();
        }
    };
}
