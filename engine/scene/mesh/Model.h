#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 
#include <assimp/scene.h>
#include <string>
#include <fstream>
#include <vector>
#include "scene/Node.h"

namespace coral
{
    class Mesh;
    struct Texture;

    class Model : public Node
    {
    public:
        // constructor, expects a filepath to a 3D model.
        Model(std::string const& path, bool gamma = false);

    private:
        // model data 
        std::string directory;
        bool gammaCorrection;

        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(std::string const& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        // process a mesh node
        std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };
}
#endif