#include "Animation.h"
#include "Bone.h"
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace coral;

Animation::Animation(const AnimationParams& params)
    : params(params)
{
}

Handle<Bone> Animation::findBone(const std::string& name) const
{
    auto it = std::find_if(params.bones.begin(), params.bones.end(), [&name](const Handle<Bone>& bone) { return bone->getBoneName() == name; });
    if (it == params.bones.end())
    {
        return nullptr;
    }
    return *it;
}

float Animation::getTicksPerSecond() const
{
    return params.ticksPerSecond;
}

float Animation::getDuration() const
{
    return params.duration;
}

Handle<AnimationNode> Animation::getRootNode() const
{
    return params.rootNode;
}

const std::unordered_map<std::string, BoneInfo>& Animation::getBonesInfoMap() const
{
    return params.boneInfoMap;
}
