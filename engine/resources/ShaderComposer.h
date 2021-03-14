#pragma once

#include "Object.h"

namespace coral
{
    struct ShaderAttribute
    {
        std::string name;
        int location;
        std::string type;
    };

    class ShaderComposer
    {
    public:
        ShaderComposer(const std::string& shaderfile);
        void addAttribute(const ShaderAttribute& attribute);
        void addDefinition(const std::string& definition);

        // Build the shaders
        void process();

        // Return the shader contents
        std::string getVertexShader() const;
        std::string getFragmentShader() const;

    private:
        std::vector<ShaderAttribute> attributes;
        std::vector<std::string> definitions;
        std::string shaderFile;
        std::string vertexContent;
        std::string fragmentContent;
    };
}
