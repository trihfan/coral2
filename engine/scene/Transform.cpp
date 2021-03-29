#include "Transform.h"
#include "Logs.h"

using namespace coral;

Transform::Transform()
    : translation(glm::vec3(0))
    , rotation(glm::quat(0, 0, 0, 0))
    , scale(glm::vec3(1))
    , matrix(1)
    , useFixedMatrix(false)
    , dirty(true)
{
    connect<&Transform::setDirtyAndNotFixedMatrix>(translation.modified, this);
    connect<&Transform::setDirtyAndNotFixedMatrix>(rotation.modified, this);
    connect<&Transform::setDirtyAndNotFixedMatrix>(scale.modified, this);
}

void Transform::setDirtyAndNotFixedMatrix()
{
    setDirty();
    useFixedMatrix = false;
}

void Transform::setDirty()
{
    dirty = true;
}

void Transform::setRotationFromEulerAngles(const glm::vec3& eulerAngles)
{
    glm::quat quaternionX = glm::angleAxis(glm::radians(eulerAngles.x), glm::vec3(1, 0, 0));
    glm::quat quaternionY = glm::angleAxis(glm::radians(eulerAngles.y), glm::vec3(0, 1, 0));
    glm::quat quaternionZ = glm::angleAxis(glm::radians(eulerAngles.z), glm::vec3(0, 0, 1));
    glm::quat finalOrientation = quaternionX * quaternionY * quaternionZ;
    rotation.set(finalOrientation);
}

void Transform::setMatrix(const glm::mat4& matrix)
{
    fixedMatrix = matrix;
    dirty = true;
    useFixedMatrix = true;
}

const glm::vec3& Transform::getPosition() const
{
    return worldPosition;
}

const glm::mat4& Transform::getMatrix() const
{
    return matrix;
}

void Transform::update(const Transform* parent)
{
    if (dirty)
    {
        if (useFixedMatrix)
        {
            matrix = (parent ? parent->getMatrix() : glm::mat4(1)) * fixedMatrix;
            worldPosition = glm::vec3(matrix * glm::vec4(0, 0, 0, 1)) + (parent ? parent->getPosition() : glm::vec3(0));
        }
        else
        {
            matrix = (parent ? parent->getMatrix() : glm::mat4(1)) * glm::translate(glm::mat4(1), translation.get()) * glm::toMat4(rotation.get()) * glm::scale(glm::mat4(1), scale.get());
            worldPosition = translation.get() + (parent ? parent->getPosition() : glm::vec3(0));
        }

        dirty = false;
        matrixChanged(matrix);
    }
}
