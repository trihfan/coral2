#ifndef MESH_MATERIAL_H
#define MESH_MATERIAL_H

#include "Material.h"
#include <memory>
#include <string>
#include <vector>

#include "backend/opengl/OpenGLBackend.h"

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

        virtual void use(const RenderParameters& parameters) override;

    protected:
        std::vector<Texture> textures;
        std::vector<GLuint> texture_ids;
        Handle<Shader> shader;
    };
}
#endif
