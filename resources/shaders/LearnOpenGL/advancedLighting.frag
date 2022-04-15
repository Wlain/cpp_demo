#version 330 core
out vec4 fragColor;

in VsOut {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fsIn;

uniform sampler2D inputTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

void main(void)
{
    vec3 color = texture(inputTexture, fsIn.texCoords).rgb;
    // Ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fsIn.fragPos);
    vec3 normal = normalize(fsIn.normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fsIn.fragPos);
    float spec = 0.0;
    if (blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = vec3(0.3) * spec;// assuming bright white light color
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}