#version 300 es

in vec3 aPosition;

uniform mat4 uViewMatrix;
uniform mat4 uProjection;

void main()
{
    vec4 worldPos = vec4(aPosition, 1.0);
    gl_Position = uProjection * uViewMatrix * worldPos;
}