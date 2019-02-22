#version 330 core

in vec2 TexCoords;

out vec3 color;

uniform sampler2D renderedTexture;
void main()
{
    color = texture(renderedTexture, TexCoords).xyz;
}
