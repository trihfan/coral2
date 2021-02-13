#ifndef PRESENTATIONMATERIAL_H
#define PRESENTATIONMATERIAL_H

#include "Material.h"
#include "Property.h"
#include <glm/glm.hpp>
#include <memory>

namespace coral
{
    class Resource;

    class PresentationMaterial : public Material
    {
    public:
        // construction
        PresentationMaterial();
        virtual ~PresentationMaterial() = default;

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) override;
    };
}
#endif