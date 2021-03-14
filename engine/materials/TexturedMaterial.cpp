#include "TexturedMaterial.h"
#include "AssetManager.h"
#include "Engine.h"
#include "renderpasses/RenderPassManager.h"
#include "resources/PipelineManager.h"
#include "scene/Node.h"
#include "scene/camera/Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

TexturedMaterial::TexturedMaterial(const std::vector<std::string>& renderQueueTags)
    : Material(renderQueueTags)
{
    enableLighting();
}

void TexturedMaterial::use(const RenderParameters& parameters)
{
    Material::use(parameters);

    // Diffuse
    int currentId = 0;
    for (size_t i = 0; i < diffuseTextures.size(); i++)
    {
        diffuseTextures[i]->bind(currentId);
        getPipeline()->setUniform("texture_diffuse" + std::to_string(i), currentId);
        currentId++;
    }

    // Specular
    for (size_t i = 0; i < specularTextures.size(); i++)
    {
        specularTextures[i]->bind(currentId);
        getPipeline()->setUniform("specular_diffuse" + std::to_string(i), currentId);
        currentId++;
    }

    // Normal
    for (size_t i = 0; i < normalTextures.size(); i++)
    {
        normalTextures[i]->bind(currentId);
        getPipeline()->setUniform("normal_diffuse" + std::to_string(i), currentId);
        currentId++;
    }

    // Height
    for (size_t i = 0; i < heightTextures.size(); i++)
    {
        heightTextures[i]->bind(currentId);
        getPipeline()->setUniform("height_diffuse" + std::to_string(i), currentId);
        currentId++;
    }
}

Handle<Pipeline> TexturedMaterial::createPipelineFor(const std::string& renderpass)
{
    PipelineParams params;
    params.renderpass = renderpass;
    params.params.name = getPipelineName();
    params.params.vertexShaderFile = AssetManager::getShader("textured_material", ShaderType::vertex).asset.url;
    params.params.fragmentShaderFile = AssetManager::getShader("textured_material", ShaderType::fragment).asset.url;
    return PipelineManager::createPipeline(params);
}

std::string TexturedMaterial::getPipelineName() const
{
    return "TexturedMaterialPipeline";
}
