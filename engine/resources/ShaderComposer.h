#pragma once

#include "base/Object.h"
#include <sstream>
#include <vector>

#define MAX_LIGHTS_COUNT 32

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
        ShaderComposer(const std::string& shaderfile, int glslVersion);
        void addAttribute(const ShaderAttribute& attribute);
        void addDefinition(const std::string& definition);

        // Build the shaders
        void process();

        // Return the shader contents
        std::string getVertexShader() const;
        std::string getFragmentShader() const;

    private:
        // Parameters
        std::string shaderFile;
        int glslVersion;
        std::vector<ShaderAttribute> attributes;
        std::vector<std::string> definitions;

        // Parser
        bool basicLighting;
        int maxLightCount;

        // Output
        std::string vertexContent;
        std::string fragmentContent;

        // Parse global shader parameters
        void parseParameters(const std::string& content);

        // Parse vertex shader specific code
        void parseVertexShader(const std::string& content);

        // Parse fragment shader specific code
        void parseFragmentShader(const std::string& content);

        // Parse the input / output from the given code
        void parseInOut(std::string& content);

        // Parse uniforms from the given code
        void parseUniforms(std::string& content);
    };
}
