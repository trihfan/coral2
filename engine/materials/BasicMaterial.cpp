#include "BasicMaterial.h"
#include "Shader.h"
#include "Engine.h"
#include "scene/Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

BasicMaterial::BasicMaterial() :
    Material("basic_material")
{
}

void BasicMaterial::use(const RenderParameters& parameters)
{
    Material::use(parameters);

    // matrix
    shader->setMat4("model", glm::translate(glm::mat4(1), glm::vec3(1, 0, 0)));
    shader->setMat4("view", parameters.camera->getViewProjectionMatrix());
    shader->setVec3("viewPos", parameters.camera->getPosition().local);

    // lights
    shader->setVec3("light.position", parameters.camera->getPosition().local);
    shader->setVec3("light.ambient", glm::vec3(1, 1, 1));
    shader->setVec3("light.diffuse", glm::vec3(1, 1, 1));
    shader->setVec3("light.specular", glm::vec3(1, 1, 1));

    // material
    shader->setVec3("material.ambient", ambient);
    shader->setVec3("material.diffuse", diffuse);
    shader->setVec3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
}