#version 330 core
out vec4 fragColor;

in vec3 normal;
in vec3 worldPos;

uniform vec3 cameraPos;
uniform samplerCube skybox;

/// 折射
void main(void)
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(worldPos - cameraPos);
    vec3 R = refract(I, normalize(normal), ratio);
    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}