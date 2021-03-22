#pragma once

#include "base/Signal.h"
#include "scene/Node.h"

namespace coral
{
    class Animation;

    /**
     * @brief The Animator class handle an animation
     */
    class Animator : public Node
    {
    public:
        // Construction
        Animator();

        // Set the animation
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

        // Signal emitted when the animation finish
        Signal<> animationFinished;

    private:
        // The animation
        Handle<Animation> animation;

        // Parameters
        double currentTime;
        bool loop;
        bool paused;
        double speed;
    };
}
