#version 300 es

precision highp float;

out vec4 FragColor;

in vec3 vNormal;  
in vec3 vFragPos;  
  
uniform vec3 uLightDir; 
uniform vec3 uViewPos; 
uniform vec3 uObjectColor;

void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

    // ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.6;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * uObjectColor;
    FragColor = vec4(result, 1.0);
} 