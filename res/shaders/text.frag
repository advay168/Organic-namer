#version 330 core
in vec2 vTexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    color = vec4(textColor, texture(text, vTexCoords).r);
}  