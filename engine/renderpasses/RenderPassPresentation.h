#pragma once

#include "RenderPass.h"
#include "scene/mesh/Mesh.h"
#include <memory>
#include <string>

namespace coral
{
    class Mesh;
    class PresentationMaterial;
    struct RenderQueue;

    class RenderPassPresentation : public RenderPass
    {
    public:
        RenderPassPresentation();

    protected:
        virtual void internalRender(RenderQueue& queue, const RenderParameters& parameters) override;

    private:
        Handle<Mesh> screenQuad;
        Handle<PresentationMaterial> presentationMaterial;
        std::unique_ptr<backend::BackendDefaultFramebuffer> defaultFramebuffer;

        void init();
        void release();
    };
}
