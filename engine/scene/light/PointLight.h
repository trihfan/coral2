#pragma once

#include "Light.h"

namespace coral
{
    class PointLight : public Light
    {
    public:
        float constant;
        float linear;
        float quadratic;
    };
}
