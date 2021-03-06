#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
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
    class Model : public Node
    {
    public:
        Model(const std::string& path);

    private:
        std::string path;
        std::string directory;
        std::unordered_map<std::string, Handle<Resource>> texturesByName;

        void init();
        void release();

        void processNode(aiNode* node, const aiScene* scene);
        Handle<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Handle<Resource>> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
    };
}
#endif
