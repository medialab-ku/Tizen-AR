#version 300 es

precision highp float;

out vec4 FragColor;

uniform float uAlpha;

in vec3 vColor;

void main()
{
    vec4 Color = vec4(vColor, uAlpha);
    FragColor = Color;
}