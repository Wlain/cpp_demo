#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D inputTexture;

void main(void)
{
    fragColor = vec4(0.0, 1.0, 0.0, 1.0);
}