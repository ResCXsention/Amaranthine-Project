#version 460 core

in vec3 ov_normal;

out vec4 of_fragment_colour;

void main()
{
	of_fragment_colour = vec4(1.0f, 0.5f, 0.5f, 1.0f);
}
