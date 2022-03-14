#include "MeshMaterial.h"
#include "AssetManager.h"
#include "FileUtils.h"
#include "resources/Pipeline.h"
#include "resources/PipelineManager.h"
#include "resources/ShaderComposer.h"
#include "scene/mesh/Mesh.h"

using namespace coral;

MeshMaterial::MeshMaterial(const std::vector<std::string>& renderQueueTags)
    : Material(renderQueueTags)
    , renderType(MeshMaterialRenderType::basic_lighting)
    , skining(false)
    , texturing(false)
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
    auto pipeline = getPipeline();

    // Basic shading
    if (renderType == MeshMaterialRenderType::basic_lighting)
    {
        pipeline->setUniform("material.ambient", ambient);
        pipeline->setUniform("material.diffuse", diffuse);
        pipeline->setUniform("material.specular", specular);
        pipeline->setUniform("material.shininess", shininess);

        int currentId = 0;
        for (size_t i = 0; i < textures.size(); i++)
        {
            for (size_t j = 0; j < textures[i].size(); j++)
            {
                textures[i][j]->bind(currentId);
                pipeline->setUniform(getTextureName(static_cast<MeshTextureType>(i), static_cast<int>(j)), currentId);
                currentId++;
            }
        }
    }

    if (skining)
    {
        for (size_t i = 0; i < maxBones; i++)
        {
            if (bones[i])
            {
                pipeline->setUniform("skeleton.finalBoneMatrices[" + std::to_string(i) + "]", bones[i]->getBoneMatrix());
            }
        }
    }
}

void MeshMaterial::setBone(int id, Handle<Bone> bone)
{
    if (id > maxBones)
    {
        Logs(warning) << "Bone id " << id << " exceed the limit of " << maxBones;
    }
    else
    {
        bones[static_cast<size_t>(id)] = bone;
    }
}

Handle<Pipeline> MeshMaterial::createPipelineFor(const std::string& renderpass)
{
    // Create pipeline
    PipelineParams params;
    params.params.name = getPipelineName();
    params.renderpass = renderpass;

    // To include in default dictionnary
    ShaderComposer composer(AssetManager::get(FileUtils::getAppDirectory() + "/assets", "shaders/mesh_material.shader").url, 450);

    // Attributes
    for (const auto& attribute : attributes)
    {
        composer.addAttribute(attribute);
    }

    // Texturing
    if (texturing)
    {
        composer.addDefinition("TEXTURING");
    }

    // Skining
    if (skining)
    {
        composer.addDefinition("SKINING");
        composer.addDefinition("MAX_BONES " + std::to_string(maxBones));
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

void MeshMaterial::enableTexturing()
{
    texturing = true;
}

void MeshMaterial::setAmbientColor(const glm::vec3& color)
{
    ambient = color;
}

void MeshMaterial::setDiffuseColor(const glm::vec3& color)
{
    diffuse = color;
}

void MeshMaterial::setSpecularColor(const glm::vec3& color)
{
    specular = color;
}

void MeshMaterial::setShininess(float shininess)
{
    this->shininess = shininess;
}

void MeshMaterial::addTexture(MeshTextureType type, Handle<Resource> resource)
{
    textures[size_t(type)].push_back(resource);
}
