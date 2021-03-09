#version 300 es

layout(location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 fTexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vPosition, 1.0);
    fTexCoords = vTexCoords;
}