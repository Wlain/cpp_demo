#version 330 core
out vec4 fragColor;

in VsOut {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fsIn;

uniform sampler2D inputTexture;

uniform vec3 lightPos[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;
uniform bool gamma;

vec3 blinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    // diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = lightColor * spec;
    // simple attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (gamma ? distance * distance : distance);
    diffuse *= attenuation;
    specular *= attenuation;
    return diffuse + specular;
}

void main(void)
{
    vec3 color = texture(inputTexture, fsIn.texCoords).rgb;
    vec3 lighting = vec3(0.0);
    for (int i = 0; i < 4; ++i)
    {
        lighting += blinnPhong(normalize(fsIn.normal), fsIn.fragPos, lightPos[i], lightColors[i]);
    }
    color *= lighting;
    if (gamma)
    {
        color = pow(color, vec3(1.0/2.2));
    }
    fragColor = vec4(color, 1.0);
}