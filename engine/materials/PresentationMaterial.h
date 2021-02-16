#ifndef PRESENTATIONMATERIAL_H
#define PRESENTATIONMATERIAL_H

#include "Material.h"

namespace coral
{
    class Resource;

    class PresentationMaterial : public Material
    {
    public:
        // construction
        PresentationMaterial();

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;
    };
}
#endif