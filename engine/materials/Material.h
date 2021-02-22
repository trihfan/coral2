#pragma once

#include "Object.h"
#include "Property.h"
#include "resources/Pipeline.h"
#include <memory>
#include <string>

namespace coral
{
    struct RenderParameters;
    class Node;

    // A material is the class containing the shader and the shader parameters
    class Material : public Object
    {
    public:
        // The material pipeline
        Property<Handle<Pipeline>> pipeline;

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) = 0;
        virtual void setNode(Handle<Node> node) = 0;
    };
}
