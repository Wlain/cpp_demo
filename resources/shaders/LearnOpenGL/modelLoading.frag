#version 330 core
out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;

void main()
{
	fragColor = texture(textureDiffuse1, texCoord) + texture(textureSpecular1, texCoord);
}