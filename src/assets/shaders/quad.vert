#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_colour;
layout (location = 2) in vec2 a_tex_coords;
layout (location = 3) in int a_tex_index;

uniform mat4 u_projection;
uniform mat4 u_transform;
uniform mat4 u_view;

out vec4 v_colour;
out vec2 v_tex_coords;
flat out int v_tex_index;

void main()
{
	v_colour = a_colour;
	v_tex_coords = a_tex_coords;
	v_tex_index = a_tex_index;
	mat4 view_projection = u_projection * u_view;
	gl_Position = view_projection * u_transform * vec4(a_position, 1);
}
