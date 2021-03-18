#pragma once

#include "Object.h"

namespace coral
{
    class Animation;
    struct AssimpNodeData;

    class Animator : public Object
    {
    public:
        Animator(Handle<Animation> Animation);

        void updateAnimation(float dt);
        void playAnimation(Animation* pAnimation);
        void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
        std::vector<glm::mat4> getFinalBoneMatrices();

    private:
        std::vector<glm::mat4> finalBoneMatrices;
        Handle<Animation> currentAnimation;
        float currentTime;
        float deltaTime;
    };
}
