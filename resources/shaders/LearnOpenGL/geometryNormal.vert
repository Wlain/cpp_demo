#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


out VsOut {
    vec3 normal;
} vsOut;

uniform mat4 view;
uniform mat4 model;

void main()
{
    vsOut.normal = vec3(vec4(mat3(transpose(inverse(view * model))) * aNormal, 0.0));
    gl_Position = view * model * vec4(aPos, 1.0);
}
