#version 460 core

in vec3 ov_normal;

out vec4 o_fragment_colour;

void main()
{
	o_fragment_colour = vec4(1.0f, 0.5f, 0.5f, 1.0f);
}
