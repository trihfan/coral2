#version 330 core
layout(location = 0) in vec3 position;

out vec2 texture_coordinates;

uniform vec2 texture_size;

void main()
{
    gl_Position = vec4(position, 1.f);
    texture_coordinates = vec2((position.x + 1) / 2., (position.y + 1) / 2.);
}
