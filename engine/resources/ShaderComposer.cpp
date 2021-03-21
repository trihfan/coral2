#include "ShaderComposer.h"
#include "BackendObjectFactory.h"
#include "Logs.h"
#include <fstream>
#include <sstream>

using namespace coral;

static const std::string basicLightingShader = R"(
uniform vec3 viewPosition;

struct PointLight
{
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLights[LIGHT_MAX];
uniform int pointLightCount;

vec3 computeLighting(vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, float shininess, vec3 normal, vec3 position)
{
    // properties
    vec3 viewDirection = normalize(viewPosition - position);

    // Ambien4
    vec3 result = ambientColor;

    // Point lights
    for (int i = 0; i < pointLightCount; i++)
    {
        vec3 lightDir = normalize(pointLights[i].position - position);

        // diffuse
        float diff = max(dot(normal, lightDir), 0.);

        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDirection, reflectDir), 0.), shininess);

        // attenuation
        float distance = length(pointLights[i].position - position);
        float attenuation = 1. / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));    

        vec3 diffuse = pointLights[i].color * diff;
        vec3 specular = pointLights[i].color * spec;

        result += diffuse * diffuseColor  + specular * specularColor * attenuation;
    }
    return result;
}
)";

enum class ParserCurrentPart
{
    parameters,
    vertex,
    fragment
};

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
    , basicLighting(false)
    , maxLightCount(32)
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

    // Content of each parts
    std::stringstream parameters, vertex, fragment;

    // Read it
    ParserCurrentPart currentPart = ParserCurrentPart::parameters;
    std::string line;
    while (std::getline(shader, line))
    {
        // Is it a title
        if (line.find("[vertex]") != std::string::npos)
        {
            currentPart = ParserCurrentPart::vertex;
            continue;
        }
        else if (line.find("[fragment]") != std::string::npos)
        {
            currentPart = ParserCurrentPart::fragment;
            continue;
        }

        switch (currentPart)
        {
        case ParserCurrentPart::parameters:
            parameters << line << std::endl;
            break;

        case ParserCurrentPart::vertex:
            vertex << line << std::endl;
            break;

        case ParserCurrentPart::fragment:
            fragment << line << std::endl;
            break;
        }
    }

    parseParameters(parameters.str());
    parseVertexShader(vertex.str());
    parseFragmentShader(fragment.str());
}

std::string ShaderComposer::getVertexShader() const
{
    return vertexContent;
}

std::string ShaderComposer::getFragmentShader() const
{
    return fragmentContent;
}

void ShaderComposer::parseParameters(const std::string& content)
{
    if (content.find("basic_lighting") != std::string::npos)
    {
        basicLighting = true;
        addDefinition("LIGHT_MAX " + std::to_string(maxLightCount));
    }
}

void ShaderComposer::parseVertexShader(const std::string& content)
{
    // Replace attribute names
    std::string str = content;
    for (const auto& attribute : attributes)
    {
        str = replaceAll(str, "VERTEX." + attribute.name, "vertex_" + attribute.name);
    }

    std::stringstream finalContent;

    // Definitions
    for (const std::string& definition : definitions)
    {
        finalContent << "#define " << definition << std::endl;
    }

    finalContent << "#define MODEL_MATRIX modelMatrix" << std::endl;
    finalContent << "#define PROJECTION_MATRIX projectionMatrix" << std::endl;
    finalContent << "#define VIEW_MATRIX viewMatrix" << std::endl;

    // Attributes
    for (const auto& attribute : attributes)
    {
        finalContent << "layout(location = " << attribute.location << ") in " << attribute.type << " vertex_" << attribute.name << ";" << std::endl;
    }

    // Uniforms
    finalContent << "uniform mat4 projectionMatrix;" << std::endl;
    finalContent << "uniform mat4 viewMatrix;" << std::endl;
    finalContent << "uniform mat4 modelMatrix;" << std::endl;

    // Finalize
    finalContent << str;
    vertexContent = finalContent.str();
}

void ShaderComposer::parseFragmentShader(const std::string& content)
{
    std::stringstream finalContent;

    // Definitions
    for (const std::string& definition : definitions)
    {
        finalContent << "#define " << definition << std::endl;
    }

    // Uniforms
    if (basicLighting)
    {
        finalContent << basicLightingShader << std::endl;
    }

    // Output
    finalContent << "out vec4 fragColor0;" << std::endl;

    // Finalize
    finalContent << content;
    fragmentContent = finalContent.str();
}
