#pragma once

#include "Animation.h"
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
    class MeshVertexBuffer;

    class Model : public Node
    {
    public:
        Model(const std::string& path);
        virtual void init() override;
        const std::vector<Animation>& getAnimations() const;

    private:
        std::string path;
        std::string directory;
        std::vector<Handle<Animation>> animations;
        std::unordered_map<std::string, Handle<Material>> materialByName;
        std::unordered_map<std::string, std::vector<Handle<Mesh>>> meshByName;
        std::unordered_map<std::string, BoneInfo> boneInfoMap;
        int boneCounter = 0;

        void loadNode(aiNode* node, const aiScene* scene);
        void loadAnimations(const aiScene* scene);
        Handle<Mesh> loadMesh(aiMesh* mesh, const aiScene* scene);
        Handle<Material> loadMaterial(aiMaterial* mat, const aiScene* scene, const MeshVertexBuffer& vertexBuffer);
        Handle<Resource> loadTexture(const aiScene* scene, const std::string& file);
    };
}
