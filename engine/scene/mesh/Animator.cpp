#include "Animator.h"
#include "Animation.h"
#include "Bone.h"
#include "Model.h"

using namespace coral;

Animator::Animator(const std::string& animationName, Handle<Model> model)
    : model(model)
{
    playAnimation(animationName);
}

void Animator::playAnimation(const std::string& animationName)
{
    this->animationName = animationName;
    animation = nullptr;
    currentTime = 0;
    finalBoneMatrices.resize(100, glm::mat4(1));
}

void Animator::update(const NodeUpdateParameters& parameters)
{
    Node::update(parameters);

    if (!animation)
    {
        animation = model->getAnimation(animationName);
    }

    if (animation)
    {
        // Pass animator to mesh material to bind matrices
        for (auto mesh : model->getChildren<Mesh>())
        {
            if (mesh->getMaterial()->isA<MeshMaterial>())
            {
                mesh->getMaterial()->toHandle<MeshMaterial>()->setAnimator(toHandle<Animator>());
            }
        }

        currentTime += static_cast<double>(animation->getTicksPerSecond()) * parameters.deltaTime;
        currentTime = std::fmod(currentTime, animation->getDuration());
        calculateBoneTransform(animation->getRootNode(), glm::mat4(1));
    }
}

void Animator::calculateBoneTransform(Handle<AnimationNode> node, const glm::mat4& parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    auto bone = animation->findBone(nodeName);
    if (bone)
    {
        bone->update(currentTime);
        nodeTransform = bone->getLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto it = animation->getBonesInfoMap().find(nodeName);
    if (it != animation->getBonesInfoMap().end())
    {
        int index = it->second.id;
        glm::mat4 offset = it->second.offset;
        finalBoneMatrices[static_cast<size_t>(index)] = globalTransformation * offset;
    }

    for (const auto& child : node->children)
    {
        calculateBoneTransform(child, globalTransformation);
    }
}

const std::vector<glm::mat4>& Animator::getFinalBoneMatrices() const
{
    return finalBoneMatrices;
}
