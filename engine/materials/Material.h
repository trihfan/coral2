#ifndef MATERIAL_H
#define MATERIAL_H

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include "Object.h"
#include "Shader.h"

namespace coral
{
    class Shader;
    struct RenderParameters;

    // A material is the class containing the shader and the shader parameters
    class Material : public Object
    {
        DECLARE_TYPE(Material)
    public:
        // return the material shader
        std::shared_ptr<Shader> getShader() const;

        // setup the shader and its parameters
        virtual void use(const RenderParameters& parameters) {};

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
