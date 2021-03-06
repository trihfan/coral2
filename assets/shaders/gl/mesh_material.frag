#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;
out vec4 fColor;

uniform sampler2D texture_diffuse1;

void main()
{
    fColor = texture(texture_diffuse1, fTexCoords);
}