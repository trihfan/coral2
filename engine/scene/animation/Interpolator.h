#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace coral
{
    /**
     * Lerp interpolator
     */
    template <typename Type>
    struct LinearInterpolator
    {
        Type operator()(const Type& value0, const Type& value1, double ratio) const;
    };

    /**
     * @brief Slerp interpolator
     */
    struct SphericalLinearInterpolator
    {
        glm::quat operator()(const glm::quat& value0, const glm::quat& value1, double ratio) const
        {
            return glm::normalize(glm::slerp(value0, value1, static_cast<float>(ratio)));
        }
    };

    // -- Implementation
    template <typename Type>
    Type LinearInterpolator<Type>::operator()(const Type& value0, const Type& value1, double ratio) const
    {
        return glm::mix(value0, value1, ratio);
    }
}
