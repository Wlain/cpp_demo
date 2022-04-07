#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform float mixValue;
uniform sampler2D inputTexture;
uniform sampler2D inputTexture1;

void main(void)
{
    fragColor = mix(texture(inputTexture, texCoord), texture(inputTexture1, texCoord), mixValue);
}