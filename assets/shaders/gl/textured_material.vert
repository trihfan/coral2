#version 330 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoords;

uniform mat4 model;
uniform mat4 view;

void main()
{
    fPosition = vec3(model * vec4(vPosition, 1));
    fNormal = mat3(transpose(inverse(model))) * vNormal;
	fTexCoords = vTexCoords;

    gl_Position = view * vec4(fPosition, 1);
}