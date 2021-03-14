#include "MeshMaterial.h"
#include "resources/Pipeline.h"
#include "resources/PipelineManager.h"
#include "resources/ShaderComposer.h"
#include "scene/mesh/Mesh.h"

using namespace coral;

MeshMaterial::MeshMaterial(const std::vector<std::string>& renderQueueTags)
    : Material(renderQueueTags)
    , renderType(MeshMaterialRenderType::basic)
{
    enableLighting();
}

void MeshMaterial::setAttributes(const std::vector<ShaderAttribute>& attributes)
{
    this->attributes = attributes;
}

void MeshMaterial::setRenderType(MeshMaterialRenderType type)
{
    renderType = type;
}

void MeshMaterial::use(const RenderParameters& parameters)
{
    Material::use(parameters);

    // Basic shading
    if (renderType == MeshMaterialRenderType::basic)
    {
        getPipeline()->setUniform("material.ambient", ambient);
        getPipeline()->setUniform("material.diffuse", diffuse);
        getPipeline()->setUniform("material.specular", specular);
        getPipeline()->setUniform("material.shininess", shininess);

        int currentId = 0;
        for (size_t i = 0; i < textures.size(); i++)
        {
            for (size_t j = 0; j < textures[i].size(); j++)
            {
                textures[i][j]->bind(currentId);
                getPipeline()->setUniform(getTextureName(static_cast<MeshTextureType>(i), static_cast<int>(j)), currentId);
                currentId++;
            }
        }
    }
}

Handle<Pipeline> MeshMaterial::createPipelineFor(const std::string& renderpass)
{
    // Create pipeline
    PipelineParams params;
    params.params.name = getPipelineName();
    params.renderpass = renderpass;

    ShaderComposer composer("assets/shaders/mesh_material.shader");

    // Attributes
    for (const auto& attribute : attributes)
    {
        composer.addAttribute(attribute);
    }

    // Skining
    if (skining)
    {
        composer.addDefinition("SKINING");
    }

    // Process
    composer.process();
    params.params.vertexShader = composer.getVertexShader();
    params.params.fragmentShader = composer.getFragmentShader();
    return PipelineManager::createPipeline(params);
}

std::string MeshMaterial::getPipelineName() const
{
    return "MeshMaterialPipeline";
}

std::string MeshMaterial::getTextureName(MeshTextureType type, int id)
{
    switch (type)
    {
    case MeshTextureType::diffuse:
        return "texture_diffuse" + std::to_string(id);

    case MeshTextureType::specular:
        return "texture_specular" + std::to_string(id);

    case MeshTextureType::normal:
        return "texture_normal" + std::to_string(id);

    case MeshTextureType::height:
        return "texture_height" + std::to_string(id);

    default:
        return "";
    }
}

void MeshMaterial::enableSkining()
{
    skining = true;
}

void MeshMaterial::setAmbientColor(const glm::vec3& ambient)
{
    this->ambient = ambient;
}

void MeshMaterial::setDiffuseColor(const glm::vec3& diffuse)
{
    this->diffuse = diffuse;
}

void MeshMaterial::setSpecularColor(const glm::vec3& specular)
{
    this->specular = specular;
}

void MeshMaterial::setShininess(float shininess)
{
    this->shininess = shininess;
}

void MeshMaterial::addTexture(MeshTextureType type, Handle<Resource> resource)
{
    textures[size_t(type)].push_back(resource);
}
