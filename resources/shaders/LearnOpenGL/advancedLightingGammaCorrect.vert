#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VsOut {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} vsOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    vsOut.texCoords = aTexCoord;
    vsOut.normal = aNormal;
    vsOut.fragPos = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}