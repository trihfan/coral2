#ifndef RENDERPARAMETERS_H
#define RENDERPARAMETERS_H

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
        std::vector<Handle<PointLight>> pointLights;
        std::vector<Handle<DirectionalLight>> directionalLights;
        std::vector<Handle<SpotLight>> spotLights;
        std::vector<Handle<AreaLight>> areaLights;
    };

    /**
     * @brief Parameters for the rendering
     */
    struct RenderParameters
    {
        // Scene
        Handle<Camera> camera;
        LightArray lights;

        // Time
        double time;
        double deltaTime;

        // Viewport
        int width;
        int height;

        void clear();
    };
}
#endif
