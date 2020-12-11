#ifndef MESH_MATERIAL_H
#define MESH_MATERIAL_H

#include <memory>
#include <string>
#include <vector>
#include <glad/glad.h>
#include "Material.h"

namespace coral
{
    class Shader;

    struct Texture
    {
        std::string type;
        std::string path;
    };

    class MeshMaterial : public Material
    {
    public:
        MeshMaterial(const std::vector<Texture>& textures);
        virtual void use() override;

    private:
        std::vector<Texture> textures;
        std::vector<GLuint> texture_ids;
        std::shared_ptr<Shader> shader;
    };
}
#endif