#include "ShaderComposer.h"
#include "BackendObjectFactory.h"
#include "Logs.h"
#include <fstream>
#include <sstream>

using namespace coral;

static const std::string basicLightingShader = R"(
struct PointLight
{
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

UNIFORM_BLOCK(Lighting)
{
    uniform vec3 viewPosition;
    uniform PointLight pointLights[LIGHT_MAX];
    uniform int pointLightCount;
} lighting;

vec3 computeLighting(vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, float shininess, vec3 normal, vec3 position)
{
    // properties
    vec3 viewDirection = normalize(lighting.viewPosition - position);

    // Ambien4
    vec3 result = ambientColor;

    // Point lights
    for (int i = 0; i < lighting.pointLightCount; i++)
    {
        vec3 lightDir = normalize(lighting.pointLights[i].position - position);

        // diffuse
        float diff = max(dot(normal, lightDir), 0.);

        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDirection, reflectDir), 0.), shininess);

        // attenuation
        float distance = length(lighting.pointLights[i].position - position);
        float attenuation = 1. / (lighting.pointLights[i].constant + lighting.pointLights[i].linear * distance + lighting.pointLights[i].quadratic * (distance * distance));    

        vec3 diffuse = lighting.pointLights[i].color * diff;
        vec3 specular = lighting.pointLights[i].color * spec;

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

static void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

static void replaceAllLayout(std::string& str, const std::string& from, const std::string& to)
{
    size_t startPos = 0;
    size_t index = 0;

    while ((startPos = str.find(from, startPos)) != std::string::npos)
    {
        std::stringstream st;
        st << "layout(location = " << index++ << ") " << to;
        std::string finalTo = st.str();

        str.replace(startPos, from.length(), finalTo);
        startPos += finalTo.length(); // Handles case where 'to' is a substring of 'from'
    }
}

ShaderComposer::ShaderComposer(const std::string& shaderfile, int glslVersion)
    : shaderFile(shaderfile)
    , glslVersion(glslVersion)
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

const std::vector<UniformBlock>& ShaderComposer::getVertexShaderUniformBlocks() const
{
    return uniformBlocks[0];
}

const std::vector<UniformBlock>& ShaderComposer::getVertexFragmentUniformBlocks() const
{
    return uniformBlocks[1];
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
        replaceAll(str, "VERTEX." + attribute.name, "vertex_" + attribute.name);
    }

    std::stringstream finalContent;

    // Definitions
    for (const std::string& definition : definitions)
    {
        finalContent << "#define " << definition << std::endl;
    }

    finalContent << "#define MODEL_MATRIX transform.modelMatrix" << std::endl;
    finalContent << "#define PROJECTION_MATRIX transform.projectionMatrix" << std::endl;
    finalContent << "#define VIEW_MATRIX transform.viewMatrix" << std::endl;
    finalContent << "#define POSITION gl_Position" << std::endl;

    // Attributes
    for (const auto& attribute : attributes)
    {
        finalContent << "IN " << attribute.type << " vertex_" << attribute.name << ";" << std::endl;
    }

    // Uniforms
    finalContent << R"(
    UNIFORM_BLOCK(Transform)
    {
        uniform mat4 projectionMatrix;
        uniform mat4 viewMatrix;
        uniform mat4 modelMatrix;
    } transform;
    )";

    // Finalize
    finalContent << str;
    vertexContent = finalContent.str();

    parseUniformBlock(vertexContent, 0);
    parseInputOutput(vertexContent);
}

void ShaderComposer::parseFragmentShader(const std::string& content)
{
    std::stringstream finalContent;

    // Definitions
    for (const std::string& definition : definitions)
    {
        finalContent << "#define " << definition << std::endl;
    }
    finalContent << "#define COLOR_0 fragColor0" << std::endl;

    // Uniforms
    if (basicLighting)
    {
        finalContent << basicLightingShader << std::endl;
    }

    // Output
    finalContent << "OUT vec4 COLOR_0;" << std::endl;

    // Finalize
    finalContent << content;
    fragmentContent = finalContent.str();

    parseUniformBlock(fragmentContent, 1);
    parseInputOutput(fragmentContent);
}

void ShaderComposer::parseUniformBlock(std::string& content, size_t index)
{
    static const std::string uniformBlockString = "UNIFORM_BLOCK(";

    int bindingCount = 0;
    size_t startPos = 0;
    while ((startPos = content.find(uniformBlockString, startPos)) != std::string::npos)
    {
        int bindingIndex = bindingCount++;

        // Create binding
        std::stringstream binding;
        if (glslVersion >= 420)
        {
            binding << "layout(binding = " << bindingIndex << ") uniform ";
        }
        else
        {
            binding << "layout(std140) uniform ";
        }
        std::string bindingStr = binding.str();

        content.replace(startPos, uniformBlockString.length(), bindingStr);
        startPos += bindingStr.length(); // Handles case where 'to' is a substring of 'from'

        // Get uniform name
        auto nameEnd = content.find(")", startPos);
        uniformBlocks[index].push_back({ content.substr(startPos, nameEnd - startPos), bindingIndex });

        // Change ( -> {
        content.replace(nameEnd, 1, "");
        startPos = nameEnd;
    }
}

void ShaderComposer::parseInputOutput(std::string& content)
{
    replaceAllLayout(content, "IN ", "in ");
    replaceAllLayout(content, "OUT ", "out ");
}