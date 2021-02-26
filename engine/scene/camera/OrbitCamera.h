#pragma once

#include "scene/camera/Camera.h"

namespace coral
{
    class OrbitCamera : public Camera
    {
    public:
        // Construction
        OrbitCamera();
        virtual ~OrbitCamera() override = default;

        /**
         * @brief Set the camera minimum and maximum distance from the view center
         */
        void setDistanceMinMax(float min, float max);

        // Mouse helpers
        void move(float x, float y);
        void zoom(float value);

    private:
        float distanceMin;
        float distanceMax;
    };
}
