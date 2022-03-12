#include "Animator.h"
#include "Animation.h"
#include "utils/Logs.h"

using namespace coral;

Animator::Animator()
    : currentTime(0)
    , loop(false)
    , paused(false)
    , speed(1)
    , totalDuration(0)
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

void Animator::addAnimation(AnimatorAnimation animation)
{
    if (!animation.animation)
    {
        Logs(error) << "Null animation";
        return;
    }

    animation.animation->start();
    animations.push_back(animation);

    // Sort by start time
    std::sort(animations.begin(), animations.end(), [](const auto& a, const auto& b) {
        return a.startTime <= b.startTime;
    });

    // Update total duration
    totalDuration = 0;
    for (const auto& animation : animations)
    {
        totalDuration = std::max(totalDuration, animation.startTime + animation.animation->getDuration());
    }
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
        currentTime = loop ? std::fmod(currentTime, getTotalDuration()) : std::clamp(currentTime, 0., getTotalDuration());
    }

    // List current animations
    std::vector<size_t> currentAnimations;
    for (size_t i = 0; i < animations.size(); i++)
    {
        if (currentTime >= animations[i].startTime && currentTime <= animations[i].startTime + animations[i].animation->getDuration())
        {
            currentAnimations.push_back(i);
        }
    }

    if (currentAnimations.size() == 1)
    {
        animations[currentAnimations[0]].animation->update(currentTime);
    }
    else if (currentAnimations.size() == 2)
    {
        // Interpolate both animations
    }
}

double Animator::getTotalDuration() const
{
    return totalDuration;
}
