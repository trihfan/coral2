#include "ShaderComposer.h"
#include "BackendObjectFactory.h"
#include "Logs.h"
#include <fstream>
#include <sstream>

using namespace coral;

static std::string replaceAll(std::string str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

ShaderComposer::ShaderComposer(const std::string& shaderfile)
    : shaderFile(shaderfile)
{
}

void ShaderComposer::addAttribute(const ShaderAttribute& attribute)
{
    attributes.push_back(attribute);
}

void ShaderComposer::addDefinition(const std::string& definition)
{
    definitions.push_back(definition);
}

void ShaderComposer::process()
{
    // Open file
    std::ifstream shader(shaderFile);
    if (!shader.is_open())
    {
        Logs(error) << "Can't open shader file";
    }

    // Prepare stream
    std::stringstream vertexShader;
    std::stringstream fragmentShader;

    // Set definitions
    for (const std::string& definition : definitions)
    {
        vertexShader << "#define " << definition << std::endl;
        fragmentShader << "#define " << definition << std::endl;
    }

    // Set keywords
    vertexShader << "#define MODEL_MATRIX modelMatrix" << std::endl;
    vertexShader << "#define PROJECTION_MATRIX projectionMatrix" << std::endl;
    vertexShader << "#define VIEW_MATRIX viewMatrix" << std::endl;

    // Set attributes
    for (const auto& attribute : attributes)
    {
        vertexShader << "layout(location = " << attribute.location << ") in " << attribute.type << " vertex_" << attribute.name << ";" << std::endl;
    }

    // Uniforms
    // Vertex matrix
    vertexShader << "uniform mat4 projectionMatrix;" << std::endl;
    vertexShader << "uniform mat4 viewMatrix;" << std::endl;
    vertexShader << "uniform mat4 modelMatrix;" << std::endl;

    // Fragment output
    fragmentShader << "out vec4 fragColor0;" << std::endl;

    // Read it
    std::string currentPartTitle;
    std::string line;
    while (std::getline(shader, line))
    {
        // Is it a title
        if (line.find_first_of('[') < line.find_first_of(']'))
        {
            size_t titleStart = line.find_first_of('[');
            size_t titleEnd = line.find_first_of(']');
            currentPartTitle = line.substr(titleStart, titleEnd - titleStart + 1);
            continue;
        }

        // Parameters
        if (currentPartTitle.empty())
        {
            // If shading enable
        }

        // Add to the correct shader
        if (currentPartTitle == "[vertex]")
        {
            vertexShader << line << std::endl;
        }
        else if (currentPartTitle == "[fragment]")
        {
            fragmentShader << line << std::endl;
        }
    }

    // Finalize
    vertexContent = vertexShader.str();
    fragmentContent = fragmentShader.str();

    // Replace attribute names
    for (const auto& attribute : attributes)
    {
        vertexContent = replaceAll(vertexContent, "VERTEX." + attribute.name, "vertex_" + attribute.name);
    }
}

std::string ShaderComposer::getVertexShader() const
{
    return vertexContent;
}

std::string ShaderComposer::getFragmentShader() const
{
    return fragmentContent;
}
