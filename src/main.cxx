#include <cstddef>
#include <cmath>
#include <string>
#include <iostream>
#include <ios>
#include <chrono>
#include <thread>
#include <vector>
#include <array>
#include <numbers>

#define GLFW_INCLUDE_NONE
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <GLFW/glfw3.h>

#include <midnight.hxx>
#include <mesh.hxx>
#include <model.hxx>
#include <resource.hxx>
#include <text_resource.hxx>

namespace glfwcbs
{
	void framebufferSizeCb(GLFWwindow *win, int w, int h)
	{
		gl::glViewport(0, 0, w, h);
	}
}

int main()
{

	const int default_win_w{800};
	const int default_win_h{600};

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *mw{glfwCreateWindow(default_win_w, default_win_h, "The Amaranthine Project", NULL, NULL)};
	glfwSetFramebufferSizeCallback(mw, glfwcbs::framebufferSizeCb);
	glfwMakeContextCurrent(mw);
	glbinding::initialize(glfwGetProcAddress);
	gl::glViewport(0, 0, default_win_w, default_win_h);

	gl::glEnable(gl::GL_CULL_FACE);
	gl::glCullFace(gl::GL_BACK);
	gl::glEnable(gl::GL_DEPTH_TEST);

	res::ResourceController<res::TextResource> rc;
	rc.index("s_vertex", "../src/vertex.glsl");
	rc.index("s_fragment", "../src/fragment.glsl");
	const char *vertex_source{rc.retrieve("s_vertex").lock()->get_text()};
	const char *fragment_source{rc.retrieve("s_fragment").lock()->get_text()};
	unsigned int program{gl::glCreateProgram()};
	unsigned int vertex_shader{gl::glCreateShader(gl::GL_VERTEX_SHADER)};
	unsigned int fragment_shader{gl::glCreateShader(gl::GL_FRAGMENT_SHADER)};
	gl::glShaderSource(vertex_shader, 1, &vertex_source, NULL);
	gl::glCompileShader(vertex_shader);
	gl::glShaderSource(fragment_shader, 1, &fragment_source, NULL);
	gl::glCompileShader(fragment_shader);
	gl::glAttachShader(program, vertex_shader);
	gl::glAttachShader(program, fragment_shader);
	gl::glLinkProgram(program);

	gl::glDeleteShader(vertex_shader);
	gl::glDeleteShader(fragment_shader);

	gl::glUseProgram(program);
	midnight::Matrix4x4 m{midnight::matrixTranslation(midnight::Vector3{0, 0, -10})};
	midnight::Matrix4x4 v{midnight::matrixIdentity<4>()};
	midnight::Matrix4x4 p{midnight::matrixPerspective(0.57, default_win_w / default_win_h, 0.001F, 2000)};
	gl::glUniformMatrix4fv(gl::glGetUniformLocation(program, "u_model"), 1, false, m.dataPtr());
	gl::glUniformMatrix4fv(gl::glGetUniformLocation(program, "u_view"), 1, false, v.dataPtr());
	gl::glUniformMatrix4fv(gl::glGetUniformLocation(program, "u_projection"), 1, false, p.dataPtr());

	const int l1{100}, l2{80};
	//res::Mesh sphere_mesh{res::spherical_mesh(1, l1, l2)};
	res::Model boat;
	boat.index_asset("boat.obj");

	while (!glfwWindowShouldClose(mw)) {
		gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

		gl::glUseProgram(program);
		boat.draw();

		const float t{static_cast<const float>(glfwGetTime())};
		midnight::Vector3 dir{midnight::cartesian3({1, std::sin(t), std::sin(t)})};
		gl::glUniform3fv(gl::glGetUniformLocation(program, "u_light_dir"), 1, dir.dataPtr());
		m *= midnight::matrixRotation(midnight::Vector3{0, 1, 0}, 0.1);
		gl::glUniformMatrix4fv(gl::glGetUniformLocation(program, "u_model"), 1, false, m.dataPtr());

		glfwSwapBuffers(mw);
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds{100});
	}

	glfwDestroyWindow(mw);
	glfwTerminate();
	return 0;
}
