#pragma once

#include "Mesh.h"
#include "materials/Material.h"
#include "resources/Resource.h"
#include "scene/Node.h"
#include <assimp/material.h>
#include <unordered_map>

class aiNode;
class aiScene;
class aiMesh;
class aiMaterial;

namespace coral
{
    struct MeshVertexBuffer;

    class Model : public Node
    {
    public:
        Model(const std::string& path);

        virtual void init() override;

    private:
        std::string path;
        std::string directory;
        std::unordered_map<std::string, Handle<Material>> materialByName;

        void loadNode(aiNode* node, const aiScene* scene);
        Handle<Mesh> loadMesh(aiMesh* mesh, const aiScene* scene);
        Handle<Material> loadMaterial(aiMaterial* mat, const MeshVertexBuffer& vertexBuffer);
    };
}
