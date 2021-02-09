#ifndef RENDERPASSDEFAULT_H
#define RENDERPASSDEFAULT_H

#include "RenderPass.h"

namespace coral
{
    struct RenderQueue;

    class RenderPassDefault : public RenderPass
    {
    public:
        RenderPassDefault();
        
    protected:
        virtual void internalRender(RenderQueue& queue) override;
    };
}
#endif