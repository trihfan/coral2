#pragma once
#include "Handle.h"
#include "Object.h"
#include <unordered_map>

class aiAnimation;
class aiNode;

namespace coral
{
    class Model;
    class Bone;

    class AnimationNode : public Object
    {
    public:
        std::string name;
        glm::mat4 transformation;
        std::vector<Handle<AnimationNode>> children;
    };

    struct BoneInfo
    {
        int id; // id is index in finalBoneMatrices
        glm::mat4 offset; // offset matrix transforms vertex from model space to bone space
    };

    struct AnimationParams
    {
        Handle<AnimationNode> rootNode;
        std::vector<Handle<Bone>> bones;
        std::unordered_map<std::string, BoneInfo> boneInfoMap;
        float duration;
        float ticksPerSecond;
    };

    class Animation : public Object
    {
    public:
        Animation(const AnimationParams& params);

        Handle<Bone> findBone(const std::string& name) const;

        float getTicksPerSecond() const;
        float getDuration() const;
        Handle<AnimationNode> getRootNode() const;
        const std::unordered_map<std::string, BoneInfo>& getBonesInfoMap() const;

    private:
        AnimationParams params;
    };
}
