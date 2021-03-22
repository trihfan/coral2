#include "MeshMaterial.h"
#include "resources/Pipeline.h"
#include "resources/PipelineManager.h"
#include "resources/ShaderComposer.h"
#include "scene/mesh/Mesh.h"
#include "scene/mesh/ModelAnimation.h"

using namespace coral;

MeshMaterial::MeshMaterial(const std::vector<std::string>& renderQueueTags)
    : Material(renderQueueTags)
    , renderType(MeshMaterialRenderType::basic_lighting)
    , skining(false)
    , texturing(false)
    , boneIncidenceCount(0)
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
    if (renderType == MeshMaterialRenderType::basic_lighting)
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

    if (skining)
    {
        if (animation && animation->getFinalBoneMatrices().size() > maxBones)
        {
            Logs(warning) << "Bone count (" << animation->getFinalBoneMatrices().size() << ") exceed the limit of " << maxBones;
        }

        size_t max = animation ? std::min(animation->getFinalBoneMatrices().size(), static_cast<size_t>(maxBones)) : maxBones;
        for (size_t i = 0; i < max; i++)
        {
            getPipeline()->setUniform("finalBoneMatrices[" + std::to_string(i) + "]", animation ? animation->getFinalBoneMatrices()[i] : glm::mat4(1));
        }
    }
}

void MeshMaterial::setAnimation(Handle<ModelAnimation> animation)
{
    this->animation = animation;
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
        for (int i = 1; i <= boneIncidenceCount; i++)
        {
            composer.addDefinition("ENABLE_BONE_INCIDENCE_" + std::to_string(i));
        }
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

void MeshMaterial::setBoneIncidenceCount(int count)
{
    boneIncidenceCount = count;
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
