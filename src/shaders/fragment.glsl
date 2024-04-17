#version 460 core

in vec3 ov_normal;

uniform vec3 u_light_dir;

out vec4 of_fragment_colour;

void main()
{
	const vec3 light_colour = vec3(1.0F, 1.0F, 1.0F);
	const vec3 material_colour = vec3(0.5F, 0.5F, 1.0F);
	const float cos_theta = clamp(dot(ov_normal, u_light_dir), 0, 1);

	const float distance = length(u_light_dir);
	const vec3 diffuse_colour = material_colour * light_colour * cos_theta / (distance * distance);
	const vec3 ambient_colour = vec3(0.1F, 0.1F, 0.1F) * material_colour;
	const vec3 colour = diffuse_colour + ambient_colour;
	//const vec3 colour = vec3(1.0F, 1.0F, 1.0F);
	of_fragment_colour = vec4(colour.xyz, 1.0F);
}
