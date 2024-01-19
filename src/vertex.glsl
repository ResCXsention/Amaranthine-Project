#version 460 core

layout (location = 0) in vec3 i_vertex;
layout (location = 1) in vec3 i_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 ov_normal;

void main()
{
	ov_normal = i_normal;
	gl_Position = u_projection * u_view * u_model * vec4(i_vertex.xyz, 1.0F);
}
