#include "Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

const glm::mat4& Camera::getViewProjectionMatrix() const
{
    return viewProjection;
}

void Camera::setFront(const glm::vec3& front)
{
    this->front = front;
    setProjectionMatrix(projection);
}

const glm::vec3& Camera::getFront() const
{
    return front;
}

void Camera::setUp(const glm::vec3& up)
{
    this->up = up;
    setProjectionMatrix(projection);
}

const glm::vec3& Camera::getUp() const
{
    return up;
}

void Camera::setProjectionMatrix(const glm::mat4& projection)
{
    this->projection = projection;
    viewProjection = projection * glm::lookAt(getPosition().local, getPosition().local + front, up);
}
