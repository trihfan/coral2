#include "Bone.h"
#include "utils/AssimpHelpers.h"
#include <assimp/anim.h>

using namespace coral;

Bone::Bone(int id, const aiNodeAnim* channel)
    : localTransform(1)
    , id(id)

{
    numPositions = channel->mNumPositionKeys;
    for (size_t positionIndex = 0; positionIndex < numPositions; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        double timeStamp = channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position = AssimpHelpers::getGLMVec(aiPosition);
        data.timeStamp = timeStamp;
        positions.push_back(data);
    }

    numRotations = channel->mNumRotationKeys;
    for (size_t rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        double timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data;
        data.orientation = AssimpHelpers::getGLMQuat(aiOrientation);
        data.timeStamp = timeStamp;
        rotations.push_back(data);
    }

    numScalings = channel->mNumScalingKeys;
    for (size_t keyIndex = 0; keyIndex < numScalings; ++keyIndex)
    {
        aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
        double timeStamp = channel->mScalingKeys[keyIndex].mTime;
        KeyScale data;
        data.scale = AssimpHelpers::getGLMVec(scale);
        data.timeStamp = timeStamp;
        scales.push_back(data);
    }
}

void Bone::update(double animationTime)
{
    animationTime *= 1000; // need animation time in ms
    glm::mat4 translation = interpolatePosition(animationTime);
    glm::mat4 rotation = interpolateRotation(animationTime);
    glm::mat4 scale = interpolateScaling(animationTime);
    localTransform = translation * rotation * scale;
}

glm::mat4 Bone::getLocalTransform()
{
    return localTransform;
}

int Bone::getBoneID()
{
    return id;
}

size_t Bone::getPositionIndex(double animationTime)
{
    for (size_t index = 0; index < numPositions - 1; index++)
    {
        if (animationTime <= positions[index + 1].timeStamp)
        {
            return index;
        }
    }
    return size_t(-1);
}

size_t Bone::getRotationIndex(double animationTime)
{
    for (size_t index = 0; index < numRotations - 1; index++)
    {
        if (animationTime <= rotations[index + 1].timeStamp)
        {
            return static_cast<size_t>(index);
        }
    }
    return size_t(-1);
}

size_t Bone::getScaleIndex(double animationTime)
{
    for (size_t index = 0; index < numScalings - 1; index++)
    {
        if (animationTime <= scales[index + 1].timeStamp)
        {
            return static_cast<size_t>(index);
        }
    }
    return size_t(-1);
}

double Bone::getScaleFactor(double lastTimeStamp, double nextTimeStamp, double animationTime)
{
    double scaleFactor = 0;
    double midWayLength = animationTime - lastTimeStamp;
    double framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::interpolatePosition(double animationTime)
{
    if (numPositions == 1)
    {
        return glm::translate(glm::mat4(1), positions[0].position);
    }

    size_t p0Index = getPositionIndex(animationTime);
    size_t p1Index = p0Index + 1;
    double scaleFactor = getScaleFactor(positions[p0Index].timeStamp, positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(positions[p0Index].position, positions[p1Index].position, scaleFactor);
    return glm::translate(glm::mat4(1), finalPosition);
}

glm::mat4 Bone::interpolateRotation(double animationTime)
{
    if (numRotations == 1)
    {
        auto rotation = glm::normalize(rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    size_t p0Index = getRotationIndex(animationTime);
    size_t p1Index = p0Index + 1;
    double scaleFactor = getScaleFactor(rotations[p0Index].timeStamp, rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation, rotations[p1Index].orientation, static_cast<float>(scaleFactor));
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolateScaling(double animationTime)
{
    if (numScalings == 1)
    {
        return glm::scale(glm::mat4(1), scales[0].scale);
    }

    size_t p0Index = getScaleIndex(animationTime);
    size_t p1Index = p0Index + 1;
    double scaleFactor = getScaleFactor(scales[p0Index].timeStamp, scales[p1Index].timeStamp, animationTime);
    glm::vec3 finalScale = glm::mix(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);
    return glm::scale(glm::mat4(1), finalScale);
}
