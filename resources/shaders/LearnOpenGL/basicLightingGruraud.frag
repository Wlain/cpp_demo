#version 330 core
in vec3 vLightColor;
out vec4 fragColor;

uniform vec3 objectColor;

void main(void)

{
    fragColor = vec4(vLightColor * objectColor, 1.0);
}