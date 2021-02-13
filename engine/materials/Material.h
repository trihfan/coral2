#ifndef MATERIAL_H
#define MATERIAL_H

#include "Object.h"
#include "resources/Shader.h"
#include <array>
#include <memory>
#include <string>
#include <unordered_map>

namespace coral
{
    class Shader;
    struct RenderParameters;

    // A material is the class containing the shader and the shader parameters
    class Material : public Object
    {
    public:
        // return the material shader
        std::shared_ptr<Shader> getShader() const;

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) = 0;

    protected:
        // constructor
        Material(const std::string& shader_name);

        // tmp
        unsigned int textureFromFile(const std::string& path, bool gamma = false);

    protected:
        // the material shader
        std::shared_ptr<Shader> shader;
    };
}
#endif
