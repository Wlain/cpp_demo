#version 330 core
in vec2 vTexCoords;

out vec4 fragColor;

uniform sampler2D textureDiffuse1;

void main()
{
    fragColor = texture(textureDiffuse1, vTexCoords);
}
