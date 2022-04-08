#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;
out vec3 vLightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
/// 在观察空间（而不是世界空间）中计算冯氏光照
void main(void)
{
    fragPos = vec3(view * model * vec4(aPos, 1.0));
    // transpose:转置
    normal = mat3(transpose(inverse(view * model))) * aNormal;
    vLightPos = vec3(view * vec4(lightPos, 1.0)); // 将世界空间灯光位置转换为视图空间灯光位置
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}