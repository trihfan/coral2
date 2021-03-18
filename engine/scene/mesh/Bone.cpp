#include "Bone.h"
#include "utils/AssimpHelpers.h"
#include <assimp/anim.h>

using namespace coral;

Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel)
    : localTransform(1.f)
    , name(name)
    , id(id)

{
    numPositions = channel->mNumPositionKeys;
    for (size_t positionIndex = 0; positionIndex < numPositions; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float timeStamp = static_cast<float>(channel->mPositionKeys[positionIndex].mTime);
        KeyPosition data;
        data.position = AssimpHelpers::getGLMVec(aiPosition);
        data.timeStamp = timeStamp;
        positions.push_back(data);
    }

    numRotations = channel->mNumRotationKeys;
    for (size_t rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float timeStamp = static_cast<float>(channel->mRotationKeys[rotationIndex].mTime);
        KeyRotation data;
        data.orientation = AssimpHelpers::getGLMQuat(aiOrientation);
        data.timeStamp = timeStamp;
        rotations.push_back(data);
    }

    numScalings = channel->mNumScalingKeys;
    for (size_t keyIndex = 0; keyIndex < numScalings; ++keyIndex)
    {
        aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
        float timeStamp = static_cast<float>(channel->mScalingKeys[keyIndex].mTime);
        KeyScale data;
        data.scale = AssimpHelpers::getGLMVec(scale);
        data.timeStamp = timeStamp;
        scales.push_back(data);
    }
}

void Bone::update(float animationTime)
{
    glm::mat4 translation = interpolatePosition(animationTime);
    glm::mat4 rotation = interpolateRotation(animationTime);
    glm::mat4 scale = interpolateScaling(animationTime);
    localTransform = translation * rotation * scale;
}

glm::mat4 Bone::getLocalTransform()
{
    return localTransform;
}

std::string Bone::getBoneName() const
{
    return name;
}

int Bone::getBoneID()
{
    return id;
}

size_t Bone::getPositionIndex(float animationTime)
{
    for (size_t index = 0; index < numPositions - 1; index++)
    {
        if (animationTime < positions[index + 1].timeStamp)
        {
            return index;
        }
    }
    return size_t(-1);
}

size_t Bone::getRotationIndex(float animationTime)
{
    for (size_t index = 0; index < numRotations - 1; index++)
    {
        if (animationTime < rotations[index + 1].timeStamp)
        {
            return static_cast<size_t>(index);
        }
    }
    return size_t(-1);
}

size_t Bone::getScaleIndex(float animationTime)
{
    for (size_t index = 0; index < numScalings - 1; index++)
    {
        if (animationTime < scales[index + 1].timeStamp)
        {
            return static_cast<size_t>(index);
        }
    }
    return size_t(-1);
}

float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor = 0.f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::interpolatePosition(float animationTime)
{
    if (numPositions == 1)
    {
        return glm::translate(glm::mat4(1.f), positions[0].position);
    }

    size_t p0Index = getPositionIndex(animationTime);
    size_t p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(positions[p0Index].timeStamp, positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(positions[p0Index].position, positions[p1Index].position, scaleFactor);
    return glm::translate(glm::mat4(1.f), finalPosition);
}

glm::mat4 Bone::interpolateRotation(float animationTime)
{
    if (numRotations == 1)
    {
        auto rotation = glm::normalize(rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    size_t p0Index = getRotationIndex(animationTime);
    size_t p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(rotations[p0Index].timeStamp, rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation, rotations[p1Index].orientation, scaleFactor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolateScaling(float animationTime)
{
    if (numScalings == 1)
    {
        return glm::scale(glm::mat4(1.f), scales[0].scale);
    }

    size_t p0Index = getScaleIndex(animationTime);
    size_t p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(scales[p0Index].timeStamp, scales[p1Index].timeStamp, animationTime);
    glm::vec3 finalScale = glm::mix(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);
    return glm::scale(glm::mat4(1.f), finalScale);
}
