#include "OrbitCamera.h"
#include <limits>

using namespace coral;

OrbitCamera::OrbitCamera()
    : distanceMin(0.1f)
    , distanceMax(std::numeric_limits<float>::max())
{
}

void OrbitCamera::setDistanceMinMax(float min, float max)
{
    assert(min > 0);
    assert(max > min);
    distanceMin = min;
    distanceMax = max;
}

void OrbitCamera::move(float x, float y)
{
    const glm::vec3 view = *position - getViewCenter();
    const glm::vec3 up = getUp();
    const float distance = glm::length(view);
    const glm::vec3 unit = view / static_cast<float>(distance);

    // Move x
    const glm::vec3 right = glm::cross(up, unit);
    const glm::vec3 movementX2D = right * -x;
    const glm::vec3 movementX = glm::normalize(*position + movementX2D - getViewCenter()) * distance - *position;

    // Move Y
    const glm::vec3 movementY2D = up * -y;
    const glm::vec3 movementY = glm::normalize(*position + movementY2D - getViewCenter()) * distance - *position;

    // Update position
    position = *position + movementX + movementY;
}

void OrbitCamera::zoom(float value)
{
    const glm::vec3 view = *position - getViewCenter();
    const float distance = glm::length(view);
    const float t = std::clamp(distance - value, distanceMin, distanceMax);
    position = getViewCenter() + view / distance * t;
}
