#pragma once
#include "base/Object.h"

namespace coral
{
    /**
     * @brief Animation interface, an animation contains a number of keyframes for any number of properties
     */
    class Animation : public Object
    {
    public:
        // Construction
        Animation() = default;
        virtual ~Animation() override = default;

        //std::vector<Property>& getProperties();
        //std::vector<KeyFrame>& getKeyframes();
        //-----

        // Return the animation duration
        virtual double getDuration() const = 0;

        // Update the animation
        virtual void update(double time) = 0;

    private:
        //std::vector<Property> properties;
        //std::vector<KeyFrame> keyframes;
    };
}
