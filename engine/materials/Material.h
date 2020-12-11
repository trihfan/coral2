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
    // A material is the class containing the shader and the shader parameters
    class Material : public Object
    {
    public:
        // setup the shader and its parameters
        virtual void use();

    protected:
        // constructor
        Material(const std::string& shader_name);

        // tmp
        unsigned int textureFromFile(const std::string& path, bool gamma = false);

        // Get the shader
        std::shared_ptr<Shader> getShader();

    private:
        // the material shader
        std::shared_ptr<Shader> shader;
    };


}
#endif
