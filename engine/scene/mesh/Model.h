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
        ptr<Animation> getAnimation(const std::string& animationName) const;

    private:
        std::string directory;
        std::vector<ptr<Animation>> animations;
        int boneCounter = 0;
        std::unordered_map<std::string, ptr<Bone>> bones;
        std::unordered_map<std::string, ptr<MeshMaterial>> materialByName;
        ptr<Node> skeleton;

        void loadNode(ptr<Node> parent, aiNode* node, const aiScene* scene);
        void loadAnimations(const aiScene* scene);
        ptr<Mesh> loadMesh(aiMesh* mesh, const aiScene* scene);
        ptr<Material> loadMaterial(aiMaterial* mat, const aiScene* scene, const MeshVertexBuffer& vertexBuffer);
        ptr<Resource> loadTexture(const aiScene* scene, const std::string& file);
        void buildSkeleton(const aiScene* scene);
    };

    class ModelAnimation : public Animation
    {
    public:
        ModelAnimation(double duration, ptr<Node> skeleton);
        void start() override;
        void stop() override;

    private:
        ptr<Node> skeleton;
    };
}
