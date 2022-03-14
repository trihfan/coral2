#include "Animation.h"

using namespace coral;

Animation::Animation(double duration)
    : duration(duration)
{
}

void Animation::addChannel(Handle<ChannelInterface> channel)
{
    channels.push_back(channel);
}

double Animation::getDuration() const
{
    return duration;
}

void Animation::update(double time)
{
    for (auto channel : channels)
    {
        channel->update(time);
    }
}

void Animation::start()
{
}

void Animation::stop()
{
}
