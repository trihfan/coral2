#include "Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

Camera::Camera()
{
    connect<&Camera::updateMatrix>(position.changed, this);
}

const glm::mat4& Camera::getViewProjectionMatrix() const
{
    return viewProjection;
}

void Camera::updateMatrix()
{
    viewProjection = projection * glm::lookAt(*position, center, up);
    viewProjectionChanged(viewProjection);
}

void Camera::setPerspective(float fovy, const glm::vec4& viewport, const glm::vec2& zNearFar)
{
    this->viewport = viewport;
    projection = glm::perspective(fovy, viewport[2] / viewport[3], zNearFar[0], zNearFar[1]);
    updateMatrix();
}

void Camera::setOrthogonal(const glm::vec4& viewport, const glm::vec2& zNearFar)
{
    this->viewport = viewport;
    projection = glm::ortho(viewport[0], viewport[0] + viewport[2], viewport[1], viewport[1] + viewport[3], zNearFar[0], zNearFar[1]);
    updateMatrix();
}

void Camera::setView(const glm::vec3& center, const glm::vec3& up)
{
    this->center = center;
    this->up = up;
    updateMatrix();
}

const glm::vec4& Camera::getViewport() const
{
    return viewport;
}

const glm::vec3& Camera::getViewCenter() const
{
    return center;
}

const glm::vec3& Camera::getUp() const
{
    return up;
}
