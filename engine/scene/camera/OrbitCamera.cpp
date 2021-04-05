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
    const glm::vec3 view = getTranslation();
    const glm::vec3 up = getUp();
    const float distance = glm::length(view);
    const glm::vec3 unit = view / static_cast<float>(distance);

    // Move x
    const glm::vec3 right = glm::cross(up, unit);
    const glm::vec3 movementX2D = right * -x;
    const glm::vec3 movementX = glm::normalize(getTranslation() + movementX2D) * distance - getTranslation();

    // Move Y
    const glm::vec3 movementY2D = up * -y;
    const glm::vec3 movementY = glm::normalize(getTranslation() + movementY2D) * distance - getTranslation();

    // Update position
    setTranslation(getTranslation() + movementX + movementY);
}

void OrbitCamera::zoom(float value)
{
    const glm::vec3 view = getTranslation() - getViewCenter();
    const float distance = glm::length(view);
    const float t = std::clamp(distance - value, distanceMin, distanceMax);
    setTranslation(getViewCenter() + view / distance * t);
}
