#version 330 core
layout (location = 0) out vec4 o_colour;

in vec4 v_colour;
in vec2 v_tex_coords;
flat in int v_tex_index;

uniform sampler2D u_textures[32];
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform sampler2D u_texture4;
uniform sampler2D u_texture5;
uniform sampler2D u_texture6;
uniform sampler2D u_texture7;
uniform sampler2D u_texture8;
uniform sampler2D u_texture9;
uniform sampler2D u_texture10;
uniform sampler2D u_texture11;
uniform sampler2D u_texture12;
uniform sampler2D u_texture13;
uniform sampler2D u_texture14;
uniform sampler2D u_texture15;

void main()
{
	vec4 t;
	switch (v_tex_index) {
	case 0: t = texture(u_texture0, v_tex_coords); break;
	case 1: t = texture(u_texture1, v_tex_coords); break;
	case 2: t = texture(u_texture2, v_tex_coords); break;
	case 3: t = texture(u_texture3, v_tex_coords); break;
	case 4: t = texture(u_texture4, v_tex_coords); break;
	case 5: t = texture(u_texture5, v_tex_coords); break;
	case 6: t = texture(u_texture6, v_tex_coords); break;
	case 7: t = texture(u_texture7, v_tex_coords); break;
	case 8: t = texture(u_texture8, v_tex_coords); break;
	case 9: t = texture(u_texture9, v_tex_coords); break;
	case 10: t = texture(u_texture10, v_tex_coords); break;
	case 11: t = texture(u_texture11, v_tex_coords); break;
	case 12: t = texture(u_texture12, v_tex_coords); break;
	case 13: t = texture(u_texture13, v_tex_coords); break;
	case 14: t = texture(u_texture14, v_tex_coords); break;
	case 15: t = texture(u_texture15, v_tex_coords); break;
	}
	o_colour = t * v_colour;
}
