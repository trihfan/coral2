#include "Camera.h"
#include <glm/gtx/transform.hpp>

using namespace coral;

Camera::Camera()
{
    connect<&Camera::updateMatrix>(projection.changed, this);
    connect<&Camera::updateMatrix>(view.changed, this);
}

const glm::mat4& Camera::getViewProjectionMatrix() const
{
    return viewProjection;
}

void Camera::updateMatrix()
{
    viewProjection = (*projection) * (*view);
    viewProjectionChanged(viewProjection);
}