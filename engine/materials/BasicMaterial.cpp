#include "BasicMaterial.h"
#include "Engine.h"
#include "resources/Shader.h"
#include "scene/camera/Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

BasicMaterial::BasicMaterial()
    : Material("basic_material")
{
}

void BasicMaterial::use(const RenderParameters& parameters)
{
    // matrix
    shader->setMat4("model", glm::mat4(1));
    shader->setMat4("view", parameters.camera->getViewProjectionMatrix());
    shader->setVec3("viewPosition", parameters.camera->getWorldPosition());

    // lights
    shader->setVec3("light.position", parameters.camera->getWorldPosition());
    shader->setVec3("light.ambient", glm::vec3(1, 1, 1));
    shader->setVec3("light.diffuse", glm::vec3(1, 1, 1));
    shader->setVec3("light.specular", glm::vec3(1, 1, 1));
    /*shader->setInt("lightCount", std::min(static_cast<int>(parameters.lights.size()), 32));
    for (size_t i = 0; i < parameters.lights.size(); i++)
    {
        auto light = parameters.lights[i];
        std::string lightStr = "light[" + std::to_string(i) + "]";
        shader->setVec3(lightStr + ".position", *light->position);
        shader->setVec3(lightStr + ".ambient", *light->ambient);
        shader->setVec3(lightStr + ".diffuse", *light->diffuse);
        shader->setVec3(lightStr + ".specular", *light->specular);
    }*/

    // material
    shader->setVec3("material.ambient", *ambient);
    shader->setVec3("material.diffuse", *diffuse);
    shader->setVec3("material.specular", *specular);
    shader->setFloat("material.shininess", *shininess);
}