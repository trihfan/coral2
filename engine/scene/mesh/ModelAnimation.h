#pragma once
#include "scene/animation/Animation.h"
#include <unordered_map>

namespace coral
{
    class Bone;
    class Model;

    /**
     * @brief The AnimationNode class contains information about an object 
     * used in the animation
     */
    class ModelAnimationNode : public Object
    {
    public:
        std::string name;
        glm::mat4 transformation;
        std::vector<Handle<ModelAnimationNode>> children;
    };

    /**
     * @brief Information anout a bone
     */
    struct BoneInfo
    {
        int id; // id is index in finalBoneMatrices
        glm::mat4 offset; // offset matrix transforms vertex from object space to bone space
    };

    /**
     * @brief Animation creation parameters
     */
    struct ModelAnimationParams
    {
        Handle<ModelAnimationNode> rootNode;
        std::vector<Handle<Bone>> bones;
        std::unordered_map<std::string, BoneInfo> boneInfoMap;
        double duration;
        double ticksPerSecond;
    };

    /**
     * @brief The ModelAnimation class handle an animation using bones
     */
    class ModelAnimation : public Animation
    {
    public:
        ModelAnimation(const ModelAnimationParams& params);
        double getDuration() const override;
        void update(double time) override;
        void setModel(Handle<Model> model);
        const std::vector<glm::mat4>& getFinalBoneMatrices() const;

    private:
        ModelAnimationParams params;
        Handle<Model> model;
        double currentTime;
        std::vector<glm::mat4> finalBoneMatrices;

        void calculateBoneTransform(Handle<ModelAnimationNode> node, const glm::mat4& parentTransform);
        Handle<Bone> findBone(const std::string& name) const;
    };
}
