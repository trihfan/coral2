#include "ModelAnimation.h"
#include "Bone.h"
#include "Mesh.h"
#include "Model.h"
#include "materials/MeshMaterial.h"

using namespace coral;

ModelAnimation::ModelAnimation(const ModelAnimationParams& params)
    : params(params)
{
    finalBoneMatrices.resize(100, glm::mat4(1));
}

double ModelAnimation::getDuration() const
{
    return params.duration;
}

const std::vector<glm::mat4>& ModelAnimation::getFinalBoneMatrices() const
{
    return finalBoneMatrices;
}

void ModelAnimation::setModel(Handle<Model> model)
{
    this->model = model;
}

void ModelAnimation::update(double time)
{
    currentTime = time;
    calculateBoneTransform(params.rootNode, glm::mat4(1));

    // Pass animator to mesh material to bind matrices
    if (model)
    {
        for (auto mesh : model->getChildren<Mesh>())
        {
            if (mesh->getMaterial()->isA<MeshMaterial>())
            {
                mesh->getMaterial()->toHandle<MeshMaterial>()->setAnimation(toHandle<ModelAnimation>());
            }
        }
    }
}

Handle<Bone> ModelAnimation::findBone(const std::string& name) const
{
    auto it = std::find_if(params.bones.begin(), params.bones.end(), [&name](const Handle<Bone>& bone) { return bone->getName() == name; });
    if (it == params.bones.end())
    {
        return nullptr;
    }
    return *it;
}

void ModelAnimation::calculateBoneTransform(Handle<ModelAnimationNode> node, const glm::mat4& parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    auto bone = findBone(nodeName);
    if (bone)
    {
        bone->update(currentTime);
        nodeTransform = bone->getLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto it = params.boneInfoMap.find(nodeName);
    if (it != params.boneInfoMap.end())
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
