#version 300 es

precision highp float;

uniform float uTime;
uniform sampler2D uTexture;
uniform float uAlpha;
uniform vec3 uLightDir;
uniform vec3 uViewPos;
uniform vec3 uLightColor;

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;

out vec4 FragColor;

float PI = 3.14159265359;

void main() 
{
    //https://forum.unity.com/threads/a-waving-texture.247458/

    // distortion
    vec2 uv = vTexCoord;
    uv.x += sin((uv.x + uv.y)*8.0 + uTime*1.3)*0.02;
    uv.y += cos(uv.y*8.0 + uTime*2.7)*0.02;

    // ambient
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * uLightColor;
    
    // diffuse
    vec3 norm = normalize(vNormal);
    // directional light
    vec3 lightDir = normalize(uLightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * uLightColor;  
        
    // increase contrast
    vec3 col = texture2D(uTexture,uv).xyz;
    col.x = pow(col.x, 4.0);
    col.y = pow(col.y, 4.0);
    col.z = pow(col.z, 4.0);

    // blurry circular edge
    float edgeClearness = 1.0;
    float distance = sqrt(pow(vTexCoord.x - 0.5, 2.0) + pow(vTexCoord.y - 0.5, 2.0));
    float t = clamp(cos(distance * (PI/2.0) / 0.5) * edgeClearness, 0.0, 1.0);
    float alpha = uAlpha * t;

    // result
    vec4 texColor = vec4(col, alpha);    
    vec4 result = vec4((ambient + diffuse + specular), 1.0) * texColor;
    
    //FragColor = result;
    FragColor = texColor;
}