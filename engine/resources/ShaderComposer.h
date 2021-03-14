#pragma once

#include "Object.h"
#include <sstream>

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
        // Parameter
        std::string shaderFile;
        std::vector<ShaderAttribute> attributes;
        std::vector<std::string> definitions;

        // Parser
        bool basicLighting;
        int maxLightCount;

        // Output
        std::string vertexContent;
        std::string fragmentContent;

        void parseParameters(const std::string& content);
        void parseVertexShader(const std::string& content);
        void parseFragmentShader(const std::string& content);
    };
}
