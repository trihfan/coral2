#pragma once

#include "RenderPass.h"

namespace coral
{
    struct RenderQueue;

    class RenderPassDefault : public RenderPass
    {
    public:
    protected:
        virtual void internalRender(RenderQueue& queue, const RenderParameters& parameters) override;
    };
}
