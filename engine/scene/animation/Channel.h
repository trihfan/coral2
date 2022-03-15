#pragma once
#include "Interpolator.h"
#include "Object.h"
#include "scene/Property.h"

namespace coral
{
    /**
     * @brief The keyframe represent a value at one timestamp
     */
    template <typename Type>
    struct KeyFrame
    {
        double timestamp;
        Type value;
    };

    /**
     * @brief Interfae for a channel
     */
    class ChannelInterface : public Object
    {
    public:
        virtual void update(double time) = 0;
    };

    /**
     * Implementation of a channel
     */
    template <typename Type, class Interpolator = LinearInterpolator<Type>>
    class Channel : public ChannelInterface
    {
    public:
        /**
         * @brief Constructor with the property modified by the schannel
         */
        Channel(Property<Type>& property);

        /**
         * @brief Add a keyframe
         */
        void addKeyframe(const KeyFrame<Type>& keyframe);

        /**
         * @brief Update the channel to the given time
         */
        void update(double time) override;

    private:
        Property<Type>& property;
        Interpolator interpolator;
        std::vector<KeyFrame<Type>> keyframes;

        size_t getIndex(double time);
        double getRatio(double timestamp0, double timestamp1, double time);
    };

    // -- Implementation
    template <typename Type, class Interpolator>
    Channel<Type, Interpolator>::Channel(Property<Type>& property)
        : property(property)
    {
    }

    template <typename Type, class Interpolator>
    void Channel<Type, Interpolator>::addKeyframe(const KeyFrame<Type>& keyframe)
    {
        keyframes.push_back(keyframe);
    }

    template <typename Type, class Interpolator>
    void Channel<Type, Interpolator>::update(double time)
    {
        Type value;
        if (keyframes.size() == 1)
        {
            value = keyframes[0].value;
        }
        else if (!keyframes.empty())
        {
            size_t index0 = getIndex(time);
            size_t index1 = index0 + 1;
            double ratio = getRatio(keyframes[index0].timestamp, keyframes[index1].timestamp, time);
            value = interpolator(keyframes[index0].value, keyframes[index1].value, ratio);
        }
        property.set(value);
    }

    template <typename Type, class Interpolator>
    size_t Channel<Type, Interpolator>::getIndex(double time)
    {
        for (size_t i = 0; i < keyframes.size() - 1; i++)
        {
            if (time <= keyframes[i + 1].timestamp)
            {
                return i;
            }
        }
        return keyframes.size() - 2;
    }

    template <typename Type, class Interpolator>
    double Channel<Type, Interpolator>::getRatio(double timestamp0, double timestamp1, double time)
    {
        double ratio = 0;
        double midWayLength = time - timestamp0;
        double framesDiff = timestamp1 - timestamp0;
        ratio = midWayLength / framesDiff;
        return std::clamp(ratio, 0., 1.);
    }
}
