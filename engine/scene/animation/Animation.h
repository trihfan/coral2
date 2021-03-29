#pragma once
#include "Channel.h"
#include "base/Object.h"
#include <vector>

namespace coral
{
    /**
     * @brief The Animation class
     */
    class Animation : public Object
    {
    public:
        Animation(double duration);
        void addChannel(ptr<ChannelInterface> channel);
        double getDuration() const;
        void update(double time);

    private:
        double duration;
        std::vector<ptr<ChannelInterface>> channels;
    };
}
