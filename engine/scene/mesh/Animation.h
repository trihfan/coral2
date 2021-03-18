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

    /**
     * @brief The AnimationNode class contains information about an object 
     * used in the animation
     */
    class AnimationNode : public Object
    {
    public:
        std::string name;
        glm::mat4 transformation;
        std::vector<Handle<AnimationNode>> children;
    };

    /**
     * @brief Information anout a bone
     */
    struct BoneInfo
    {
        int id; // id is index in finalBoneMatrices
        glm::mat4 offset; // offset matrix transforms vertex from model space to bone space
    };

    /**
     * @brief Animation creation parameters
     */
    struct AnimationParams
    {
        std::string name;
        Handle<AnimationNode> rootNode;
        std::vector<Handle<Bone>> bones;
        std::unordered_map<std::string, BoneInfo> boneInfoMap;
        double duration;
        double ticksPerSecond;
    };

    /**
     * @brief The Animation class contains information about an animation of a model using bones
     */
    class Animation : public Object
    {
    public:
        Animation(const AnimationParams& params);

        const std::string& getName() const;
        Handle<Bone> findBone(const std::string& name) const;

        double getTicksPerSecond() const;
        double getDuration() const;
        Handle<AnimationNode> getRootNode() const;
        const std::unordered_map<std::string, BoneInfo>& getBonesInfoMap() const;

    private:
        AnimationParams params;
    };
}
