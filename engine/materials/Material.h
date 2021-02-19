#pragma once

#include "Object.h"
#include "resources/Shader.h"
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
        // the material shader
        Handle<Shader> shader;

        // return the material shader
        Handle<Shader> getShader() const;

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) = 0;
        virtual void setNode(Handle<Node> node) = 0;

    protected:
        // constructor
        Material(const std::string& shader_name);

        // tmp
        unsigned int textureFromFile(const std::string& path, bool gamma = false);
    };
}
