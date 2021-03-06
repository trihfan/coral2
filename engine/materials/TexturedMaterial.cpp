#include "TexturedMaterial.h"
#include "AssetManager.h"
#include "Engine.h"
#include "renderpasses/RenderPassManager.h"
#include "resources/PipelineManager.h"
#include "scene/Node.h"
#include "scene/camera/Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

TexturedMaterial::TexturedMaterial()
{
    PipelineParams params;
    params.name = "textured_material";
    params.renderpass = defaultRenderPassName;
    params.vertexShaderFile = AssetManager::getShader("textured_material", ShaderType::vertex).asset.url;
    params.fragmentShaderFile = AssetManager::getShader("textured_material", ShaderType::fragment).asset.url;

    pipeline = PipelineManager::getPipeline(params);
}

void TexturedMaterial::use(const RenderParameters& parameters)
{
    setupLights(parameters);

    pipeline->setUniform("view", parameters.camera->getViewProjectionMatrix());
    pipeline->setUniform("viewPosition", parameters.camera->getWorldPosition());

    // Diffuse
    int currentId = 0;
    for (size_t i = 0; i < diffuseTextures.size(); i++)
    {
        diffuseTextures[i]->bind(currentId);
        pipeline->setUniform("texture_diffuse" + std::to_string(i), currentId);
        currentId++;
    }

    // Specular
    for (size_t i = 0; i < specularTextures.size(); i++)
    {
        specularTextures[i]->bind(currentId);
        pipeline->setUniform("specular_diffuse" + std::to_string(i), currentId);
        currentId++;
    }

    // Normal
    for (size_t i = 0; i < normalTextures.size(); i++)
    {
        normalTextures[i]->bind(currentId);
        pipeline->setUniform("normal_diffuse" + std::to_string(i), currentId);
        currentId++;
    }

    // Height
    for (size_t i = 0; i < heightTextures.size(); i++)
    {
        heightTextures[i]->bind(currentId);
        pipeline->setUniform("height_diffuse" + std::to_string(i), currentId);
        currentId++;
    }
}

void TexturedMaterial::setNode(Handle<Node> node)
{
    pipeline->setUniform("model", node->getWorldMatrix());
}
