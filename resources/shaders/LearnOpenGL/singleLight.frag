#version 330 core
out vec4 fragColor;

uniform vec3 lightColor;

void main(void)
{
    fragColor = vec4(lightColor, 1.0);
}