#include "BasicMaterial.h"
#include "Shader.h"
#include "Engine.h"
#include "scene/Camera.h"

using namespace coral;

BasicMaterial::BasicMaterial() :
    Material("basic_material")
{
}

void BasicMaterial::use()
{
    Material::use();

    // matrix
    shader->setMat4("model", glm::mat4());
    shader->setMat4("viewProjection", current_parameters.camera->getViewProjectionMatrix());

    // lights

    // material
    shader->setVec3("material.ambient", ambient);
    shader->setVec3("material.diffuse", diffuse);
    shader->setVec3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
}