#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 vLightColor;

uniform vec3 lightColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;

/// gouraud shading
void main(void)
{
    // transpose:转置
    vec3 worldPos = vec3(model * vec4(aPos, 1.0));
    vec3 normal = mat3(transpose(inverse(view * model))) * aNormal;
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    // diffuse
    vec3 norm = normalize(aNormal);
    vec3 lightDir = normalize(lightPos - worldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vLightColor = ambient + diffuse + specular;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}