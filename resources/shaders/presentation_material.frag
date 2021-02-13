#version 330 core

out vec4 color;
uniform sampler2D backbuffer;

void main()
{
    color = texelFetch(backbuffer, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
}