#version 330 core

uniform sampler2D tex;
uniform vec2 center;
uniform float scale;


layout(location = 0) out vec3 color;

void main()
{
    color = vec3(1.0, 1.0, 1.0);
}
