#version 330 core

#define LIGHTS_COUNT 32

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fPosition;
in vec3 fNormal;
out vec4 fColor;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main()
{
    // properties
    vec3 normal = normalize(fNormal);
    vec3 viewDirection = normalize(viewPosition - fPosition);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 lightDir = normalize(light.position - fPosition);
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    fColor = vec4(result, 1);
}