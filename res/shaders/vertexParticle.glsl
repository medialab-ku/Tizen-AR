#version 300 es

in vec3 aPosition;
in vec2 aTexCoord;
in vec4 aColor;

uniform mat4 uMvpMatrix;
uniform vec3 uSize;
uniform mat3 uNormalMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uModelView;
uniform mat4 uViewMatrix;

out vec2 vTexCoord;
out vec4 vColor;

void main()
{   
    vec4 vertexPosition = vec4(aPosition, 1.0);
    vertexPosition.xyz *= uSize;

    vTexCoord = aTexCoord;
    vColor = aColor;

    gl_Position = uMvpMatrix * vertexPosition;
}
    