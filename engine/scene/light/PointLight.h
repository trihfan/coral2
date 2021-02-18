#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

namespace coral
{
    class PointLight : public Light
    {
    public:
        Property<float> constant;
        Property<float> linear;
        Property<float> quadratic;
    };
}
#endif
