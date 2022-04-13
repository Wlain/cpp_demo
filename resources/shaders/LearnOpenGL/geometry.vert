#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out VsOut {
    vec3 color;
} vsOut;

void main()
{
    vsOut.color = aColor;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}
