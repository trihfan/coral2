#pragma once

#include "Material.h"
#include "resources/Resource.h"
#include "resources/ShaderComposer.h"
#include <vector>

namespace coral
{
    struct MeshVertexBuffer;

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

    protected:
        virtual Handle<Pipeline> createPipelineFor(const std::string& renderpass) override;
        virtual std::string getPipelineName() const override;

    private:
        MeshMaterialRenderType renderType;
        bool skining;
        bool texturing;
        std::vector<ShaderAttribute> attributes;

        // uniforms
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
        std::array<std::vector<Handle<Resource>>, MeshTextureType::count> textures;

        std::string getTextureName(MeshTextureType type, int id);
    };
}
