#include "Animator.h"
#include "Animation.h"
#include "Bone.h"
#include "Model.h"

using namespace coral;

Animator::Animator(const std::string& animationName, Handle<Model> model)
    : model(model)
    , currentTime(0)
    , loop(false)
    , paused(false)
{
    setAnimation(animationName);
}

void Animator::play()
{
    paused = false;
}

void Animator::pause()
{
    paused = true;
}

void Animator::restart()
{
    currentTime = 0;
}

void Animator::setTime(double animationTime)
{
    currentTime = animationTime;
}

void Animator::setAnimation(const std::string& animationName)
{
    this->animationName = animationName;
    animation = nullptr;
    finalBoneMatrices.resize(100, glm::mat4(1));
    restart();
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

        // Advance animatino time
        if (!paused)
        {
            currentTime += parameters.deltaTime * 1000;
            if (currentTime >= animation->getDuration())
            {
                animationFinished();
            }
        }

        // Clamp animation time
        if (loop)
        {
            currentTime = std::fmod(currentTime, animation->getDuration());
        }
        else
        {
            currentTime = std::clamp(currentTime, 0., animation->getDuration() - 1);
        }
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

void Animator::setLoopAnimation(bool loop)
{
    this->loop = loop;
}
