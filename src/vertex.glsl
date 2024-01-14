#version 460 core

layout (location = 0) in vec3 i_vertex;

layout (location = 100) uniform mat4 u_model;
layout (location = 101) uniform mat4 u_view = mat4(1.0);
layout (location = 102) uniform mat4 u_projection = mat4(1.0);

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(i_vertex.xyz, 1.0F);
}
