#version 300 es

in vec3 aPosition;
in vec2 aTexCoord;
in vec3 aNormal;

uniform mat4 uMvpMatrix;
uniform vec3 uSize;
uniform mat3 uNormalMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uModelView;
uniform mat4 uViewMatrix;

out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTexCoord;

void main()
{   
    vec4 vertexPosition = vec4(aPosition, 1.0);
    vertexPosition.xyz *= uSize;

    vTexCoord = aTexCoord;
    vFragPos = vec3(uModelMatrix * vertexPosition);
    vNormal = (mat3(transpose(inverse(uModelMatrix))) * aNormal);

    gl_Position = uMvpMatrix * vertexPosition;
}
    