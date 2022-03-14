#pragma once

#include "Mesh.h"
#include "materials/MeshMaterial.h"
#include "scene/Node.h"
#include "scene/animation/Animation.h"
#include <assimp/material.h>
#include <unordered_map>

class aiNode;
class aiScene;
class aiMesh;
class aiMaterial;

namespace coral
{
    class MeshVertexBuffer;
    class Resource;

    struct BoneInfo
    {
        int id; // id is index in finalBoneMatrices
        glm::mat4 offset; // offset matrix transforms vertex from object space to bone space
    };

    class Model : public Node
    {
    public:
        // Creation
        Model(const std::string& path);

        void update(const NodeUpdateParameters& parameters) override;

        // Animations
        std::vector<std::string> getAnimationNames() const;
        Handle<Animation> getAnimation(const std::string& animationName) const;

    private:
        std::string directory;
        std::vector<Handle<Animation>> animations;
        int boneCounter = 0;
        std::unordered_map<std::string, Handle<Bone>> bones;
        std::unordered_map<std::string, Handle<MeshMaterial>> materialByName;
        Handle<Node> skeleton;

        void loadNode(Handle<Node> parent, aiNode* node, const aiScene* scene);
        void loadAnimations(const aiScene* scene);
        Handle<Mesh> loadMesh(aiMesh* mesh, const aiScene* scene);
        Handle<Material> loadMaterial(aiMaterial* mat, const aiScene* scene, const MeshVertexBuffer& vertexBuffer);
        Handle<Resource> loadTexture(const aiScene* scene, const std::string& file);
        void buildSkeleton(const aiScene* scene);
    };

    class ModelAnimation : public Animation
    {
    public:
        ModelAnimation(double duration, Handle<Node> skeleton);
        void start() override;
        void stop() override;

    private:
        Handle<Node> skeleton;
    };
}
