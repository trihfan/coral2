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
    const glm::vec3 view = translation.get() - getViewCenter();
    const glm::vec3 up = getUp();
    const float distance = glm::length(view);
    const glm::vec3 unit = view / static_cast<float>(distance);

    // Move x
    const glm::vec3 right = glm::cross(up, unit);
    const glm::vec3 movementX2D = right * -x;
    const glm::vec3 movementX = glm::normalize(translation.get() + movementX2D - getViewCenter()) * distance - translation.get();

    // Move Y
    const glm::vec3 movementY2D = up * -y;
    const glm::vec3 movementY = glm::normalize(translation.get() + movementY2D - getViewCenter()) * distance - translation.get();

    // Update position
    translation.set(translation.get() + movementX + movementY);
}

void OrbitCamera::zoom(float value)
{
    const glm::vec3 view = translation.get() - getViewCenter();
    const float distance = glm::length(view);
    const float t = std::clamp(distance - value, distanceMin, distanceMax);
    translation.set(getViewCenter() + view / distance * t);
}
