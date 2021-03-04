#version 300 es

#define LIGHT_MAX 32

struct Material
{
    mediump vec3 color;
    mediump float shininess;
};

struct PointLight
{
    mediump vec3 position;
    mediump vec3 color;
    mediump float constant;
    mediump float linear;
    mediump float quadratic;
};

struct AmbientLight
{
    mediump float t;
};

in vec3 fPosition;
in vec3 fNormal;
//in vec2 fTextureCoordinates;
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
    vec3 result = vec3(0.3, 0.3, 0.3);

    // Point lights
    for (int i = 0; i < pointLightCount; i++)
    {
        vec3 lightDir = normalize(pointLights[i].position - fPosition);

        // diffuse
        float diff = max(dot(normal, lightDir), 0);

        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);

        // attenuation
        float distance = length(pointLights[i].position - fPosition);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));    

        vec3 diffuse = pointLights[i].color * diff;
        vec3 specular = pointLights[i].color * spec;

        result += (diffuse + specular) * material.color * attenuation;
    }

    fColor = vec4(result, 1);
}