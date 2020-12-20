#include "BasicMaterial.h"
#include "Shader.h"
#include "Engine.h"
#include "scene/Camera.h"

using namespace coral;

BasicMaterial::BasicMaterial() :
    Material("basic_material")
{
}

void BasicMaterial::use(const RenderParameters& parameters)
{
    Material::use(parameters);

    // matrix
    shader->setMat4("model", glm::mat4());
    shader->setMat4("viewProjection", parameters.camera->getViewProjectionMatrix());
    shader->setVec3("viewPos", parameters.camera->getPosition());

    // lights
    shader->setVec3("light.position", parameters.camera->getPosition());
    shader->setVec3("light.ambient", glm::vec3(1, 1, 1));
    shader->setVec3("light.diffuse", glm::vec3(1, 1, 1));
    shader->setVec3("light.specular", glm::vec3(1, 1, 1));

    // material
    shader->setVec3("material.ambient", ambient);
    shader->setVec3("material.diffuse", diffuse);
    shader->setVec3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
}