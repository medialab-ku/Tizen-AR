#version 300 es

in vec3 aPosition;
in vec3 aColor;

uniform mat4 uMvpMatrix;
uniform vec3 uSize;

out vec3 vColor;

void main()
{
    vColor = aColor;
    vec4 vertexPosition = vec4(aPosition, 1.0);
    vertexPosition.xyz *= uSize;
    gl_Position = uMvpMatrix * vertexPosition;
}