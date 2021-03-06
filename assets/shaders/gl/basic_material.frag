#version 330 core

#define LIGHT_MAX 32

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight
{
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct AmbientLight
{
    float t;
};

in vec3 fPosition;
in vec3 fNormal;

out vec4 fColor;

uniform vec3 viewPosition;
uniform Material material;

// Lighting
uniform PointLight pointLights[LIGHT_MAX];
uniform int pointLightCount;

uniform AmbientLight ambientLight;

void main()
{
    // properties
    vec3 normal = normalize(fNormal);
    vec3 viewDirection = normalize(viewPosition - fPosition);

    // Ambien4
    vec3 result = material.ambient;

    // Point lights
    for (int i = 0; i < pointLightCount; i++)
    {
        vec3 lightDir = normalize(pointLights[i].position - fPosition);

        // diffuse
        float diff = max(dot(normal, lightDir), 0.);

        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDirection, reflectDir), 0.), material.shininess);

        // attenuation
        float distance = length(pointLights[i].position - fPosition);
        float attenuation = 1. / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));    

        vec3 diffuse = pointLights[i].color * diff;
        vec3 specular = pointLights[i].color * spec;

        result += diffuse * material.diffuse  + specular * material.specular * attenuation;
    }

    fColor = vec4(result, 1);
}