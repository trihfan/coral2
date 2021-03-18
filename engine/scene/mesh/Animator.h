#pragma once

#include "scene/Node.h"

namespace coral
{
    class Animation;
    class AnimationNode;
    class Model;

    class Animator : public Node
    {
    public:
        Animator(const std::string& animationName, Handle<Model> model);

        void playAnimation(const std::string& animationName);

        virtual void update(const NodeUpdateParameters& parameters) override;
        void calculateBoneTransform(Handle<AnimationNode> node, const glm::mat4& parentTransform);
        const std::vector<glm::mat4>& getFinalBoneMatrices() const;

    private:
        std::vector<glm::mat4> finalBoneMatrices;
        std::string animationName;
        Handle<Animation> animation;
        Handle<Model> model;
        double currentTime;
    };
}
