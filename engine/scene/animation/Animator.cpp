#include "Animator.h"
#include "Animation.h"

using namespace coral;

Animator::Animator()
    : currentTime(0)
    , loop(false)
    , paused(false)
    , speed(1)
{
}

void Animator::play()
{
    paused = false;
}

void Animator::pause()
{
    paused = true;
}

void Animator::restart()
{
    currentTime = 0;
}

void Animator::setCurrentTime(double currentTime)
{
    this->currentTime = currentTime;
}

void Animator::setSpeed(double speed)
{
    this->speed = speed;
}

void Animator::setAnimation(Handle<Animation> animation)
{
    this->animation = animation;
    restart();
}

void Animator::setLoopAnimation(bool loop)
{
    this->loop = loop;
}

void Animator::update(const NodeUpdateParameters& parameters)
{
    Node::update(parameters);

    // Advance animation time
    if (!paused)
    {
        currentTime += parameters.deltaTime * speed;
    }

    if (animation)
    {
        // Clamp animation time
        currentTime = loop ? std::fmod(currentTime, animation->getDuration()) : std::clamp(currentTime, 0., animation->getDuration());

        // Update each properties
        animation->update(currentTime);
    }
}
