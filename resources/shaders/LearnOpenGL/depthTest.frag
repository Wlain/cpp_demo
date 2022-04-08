#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D inputTexture;

void main(void)
{
    vec4 color = texture(inputTexture, texCoord);
    if(color.a < 0.1)
    {
        discard;
    }
    fragColor = color;
}