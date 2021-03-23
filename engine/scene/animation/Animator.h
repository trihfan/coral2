#pragma once

#include "Animation.h"
#include "scene/Node.h"

namespace coral
{
    /**
     * @brief The Animator class handle an animation
     */
    class Animator : public Node
    {
    public:
        // Construction
        Animator();

        // Add an animation to the list
        void setAnimation(Handle<Animation> animation);

        // Animation playing
        void play();
        void pause();
        void restart();

        // Animation parameters
        void setCurrentTime(double currentTime);
        void setSpeed(double speed);
        void setLoopAnimation(bool loop);

        // Update the animation, called each frame
        virtual void update(const NodeUpdateParameters& parameters) override;

    private:
        // The animations
        Handle<Animation> animation;

        // Parameters
        double currentTime;
        bool loop;
        bool paused;
        double speed;
    };
}
