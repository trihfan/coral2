#version 300 es
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

out vec3 fPosition;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;

void main()
{
    fPosition = vec3(model * vec4(vPosition, 1));
    fNormal = mat3(transpose(inverse(model))) * vNormal;

    gl_Position = view * vec4(fPosition, 1);
}