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
        void addChannel(Handle<ChannelInterface> channel);
        double getDuration() const;
        void update(double time);

        // Mehods called when an animator start or stop animating
        virtual void start();
        virtual void stop();

    private:
        double duration;
        std::vector<Handle<ChannelInterface>> channels;
    };
}
