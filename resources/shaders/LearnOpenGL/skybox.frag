#version 330 core

in vec3 texcoord;
out vec4 fragColor;

uniform samplerCube skybox;

void main() {
    fragColor = texture(skybox, texcoord);
}