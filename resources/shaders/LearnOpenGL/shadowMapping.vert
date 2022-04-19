#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;

out VsOut {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec4 fragPosLightSpace;
} vsOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main(void)
{
    vsOut.fragPos = vec3(model * vec4(aPos, 1.0));
    vsOut.normal = normalize(transpose(inverse(mat3(model))) * aNormal);
    vsOut.texCoords = aTexCoords;
    vsOut.fragPosLightSpace = lightSpaceMatrix * vec4(vsOut.fragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}