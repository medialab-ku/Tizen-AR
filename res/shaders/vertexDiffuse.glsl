#version 300 es

in vec3 aPosition;
in vec3 aNormal;

out vec3 vFragPos;
out vec3 vNormal;

uniform mat4 uModelMatrix;
uniform mat4 uMvpMatrix;
uniform vec3 uSize;

void main()
{
    vec4 vertexPosition = vec4(aPosition, 1.0);
    vertexPosition.xyz *= uSize;

    vFragPos = vec3(uModelMatrix * vertexPosition);
    vNormal = (mat3(transpose(inverse(uModelMatrix))) * aNormal);

    gl_Position = uMvpMatrix * vertexPosition;
}