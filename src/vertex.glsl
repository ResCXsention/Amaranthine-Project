#version 460 core

layout (location = 0) in vec3 i_vertex;
layout (location = 1) in vec3 i_normal;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);

out vec3 ov_normal;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(i_vertex.xyz, 1.0F);
}
