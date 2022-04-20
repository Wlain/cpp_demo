#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;
layout (location = 0) in vec2 aTexCooord;

out VsOut {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec4 fragPosLightSpace;
} vsOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool reverseNormals;
// 一个小技巧，以确保外部大立方体显示灯光从内部，而不是默认的外部。
void main()
{
    vsOut.fragPos = vec3(model * vec4(aPos, 1.0));
    vsOut.normal = normalize(transpose(inverse(mat3(model))) * aNormal * reverseNormals ? -1.0 : 1.0);
    vsOut.texCoords = aTexCoords;
    vsOut.fragPosLightSpace = lightSpaceMatrix * vec4(vsOut.fragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
