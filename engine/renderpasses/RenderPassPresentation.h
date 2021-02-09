#ifndef RENDERPASSPRESENTATION_H
#define RENDERPASSPRESENTATION_H

#include <string>
#include "RenderPass.h"

namespace coral
{
    struct RenderQueue;

    class RenderPassPresentation : public RenderPass
    {
    public:
        void setFinalFramebuffer(const std::string& framebuffer);
        
    protected:
        virtual void internalRender(RenderQueue& queue) override;

    private:
        std::string framebuffer;
    };
}
#endif