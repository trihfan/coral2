#pragma once

#include "Material.h"
#include "Handle.h"
#include "resources/Resource.h"
#include "scene/mesh/Bone.h"
#include <array>
#include <vector>

namespace coral
{
    struct MeshVertexBuffer;
    struct ShaderAttribute;

    enum MeshMaterialRenderType
    {
        basic_lighting
    };

    enum MeshTextureType : int
    {
        diffuse,
        specular,
        normal,
        height,
        count,
        unknown = count
    };

    class MeshMaterial : public Material
    {
    public:
        /**
         * @brief Maximum number of bones per model
         */
        static constexpr int maxBones = 150;

        MeshMaterial(const std::vector<std::string>& renderQueueTags);

        // Setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;

        // parameters
        void setAttributes(const std::vector<ShaderAttribute>& attributes);
        void setRenderType(MeshMaterialRenderType type);
        void enableTexturing();
        void enableSkining();

        // Uniforms
        void setAmbientColor(const glm::vec3& color);
        void setDiffuseColor(const glm::vec3& color);
        void setSpecularColor(const glm::vec3& color);
        void setShininess(float shininess);
        void addTexture(MeshTextureType type, Handle<Resource> resource);

        // Set a bone
        void setBone(int id, Handle<Bone> bone);

    protected:
        virtual Handle<Pipeline> createPipelineFor(const std::string& renderpass) override;
        virtual std::string getPipelineName() const override;

    private:
        MeshMaterialRenderType renderType;
        bool skining;
        bool texturing;
        std::vector<ShaderAttribute> attributes;

        // Uniforms
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
        std::array<std::vector<Handle<Resource>>, MeshTextureType::count> textures;

        // Bones
        std::array<Handle<Bone>, maxBones> bones;

        std::string getTextureName(MeshTextureType type, int id);
    };
}
