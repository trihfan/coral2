#version 300 es
precision highp float;

in vec2 fTexCoords;

out vec4 fColor;

uniform sampler2D glyph;
uniform vec3 color;

void main()
{    
    fColor = vec4(color, texture(glyph, fTexCoords).r);
}
