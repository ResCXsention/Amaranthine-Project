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
#include <resource.hxx>
#include <node.hxx>
#include <scene.hxx>
#include <component.hxx>
#include <drawable_component.hxx>
#include <camera_component.hxx>
#include <text_resource.hxx>
#include <model_resource.hxx>

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

	{
		res::ResourceController<res::TextResource> rc;
		rc.index("s_vertex", "../src/shaders/vertex.glsl");
		rc.index("s_fragment", "../src/shaders/fragment.glsl");
		rc.index("s_water_fragment", "../src/shaders/water_fragment.glsl");
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
		unsigned int water_program{gl::glCreateProgram()};
		const char *water_fragment_source{rc.retrieve("s_water_fragment").lock()->get_text()};
		unsigned int water_fragment_shader{gl::glCreateShader(gl::GL_FRAGMENT_SHADER)};
		gl::glShaderSource(water_fragment_shader, 1, &water_fragment_source, NULL);
		gl::glCompileShader(water_fragment_shader);
		gl::glAttachShader(water_program, vertex_shader);
		gl::glAttachShader(water_program, water_fragment_shader);
		gl::glLinkProgram(water_program);
		gl::glDeleteShader(vertex_shader);
		gl::glDeleteShader(fragment_shader);
		gl::glDeleteShader(water_fragment_shader);

		gl::glUseProgram(program);
		midnight::Matrix4x4 m{midnight::matrixTranslation(midnight::Vector3{0, 0, -3})};
		m *= midnight::matrixRotation(midnight::Vector3{-1, 0, 0}, 0.1);
		midnight::Matrix4x4 v{midnight::matrixIdentity<4>()};
		midnight::Matrix4x4 p{midnight::matrixPerspective(0.57, default_win_w / default_win_h, 0.001F, 2000)};

		res::Scene main_scene;

		res::Node *hull{main_scene.get_root()->add_child()}, *rear_turret{hull->add_child()}, *forward_turret{hull->add_child()};
		hull->add_component<res::Drawable>();
		rear_turret->add_component<res::Drawable>();
		forward_turret->add_component<res::Drawable>();
		hull->get_component<res::Drawable>()->set_shader(program);
		rear_turret->get_component<res::Drawable>()->set_shader(program);
		forward_turret->get_component<res::Drawable>()->set_shader(program);

		res::Node *camera{main_scene.get_root()->add_child()};
		camera->add_component<res::Camera>();
		camera->get_component<res::Camera>()->set_active();
		camera->get_component<res::Camera>()->set_fov(2);
		
		res::ResourceController<res::ModelResource> mc;
		mc.index("m_boat", "boat.obj");
		mc.index("m_ags", "ags.obj");
		mc.index("m_hollow", "hollow.obj");
		hull->get_component<res::Drawable>()->set_model(mc.retrieve("m_boat"));
		hull->get_component<res::Drawable>()->set_model(mc.retrieve("m_boat"));
		rear_turret->get_component<res::Drawable>()->set_model(mc.retrieve("m_ags"));
		forward_turret->get_component<res::Drawable>()->set_model(mc.retrieve("m_ags"));
		
		hull->set_main_transform(m);
		rear_turret->set_priority_transform(midnight::matrixRotation(midnight::Vector3{0, 1, 0}, std::numbers::pi_v<float> / -4));
		forward_turret->set_priority_transform(midnight::matrixRotation(midnight::Vector3{0, 1, 0}, std::numbers::pi_v<float> / 3));
		rear_turret->set_main_transform(midnight::matrixTranslation(midnight::Vector3{-0.61, 0.15, 0}));
		forward_turret->set_main_transform(midnight::matrixTranslation(midnight::Vector3{-1.15, 0.15, 0}));

		while (!glfwWindowShouldClose(mw)) {
			gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

			if (glfwGetKey(mw, GLFW_KEY_W) == GLFW_PRESS) {
				camera->transform_main(midnight::matrixTranslation(midnight::Vector3{0, 0, -0.5}));
			}  else if (glfwGetKey(mw, GLFW_KEY_S) == GLFW_PRESS) {
				camera->transform_main(midnight::matrixTranslation(midnight::Vector3{0, 0, 0.5}));
			} else if (glfwGetKey(mw, GLFW_KEY_A) == GLFW_PRESS) {
				camera->transform_main(midnight::matrixTranslation(midnight::Vector3{-0.5, 0, 0}));
			}  else if (glfwGetKey(mw, GLFW_KEY_D) == GLFW_PRESS) {
				camera->transform_main(midnight::matrixTranslation(midnight::Vector3{0.5, 0, 0}));
			} 

			const float t{static_cast<const float>(glfwGetTime())};
			midnight::Vector3 dir{midnight::cartesian3({1, std::sin(t), std::sin(t)})};
			gl::glUniform3fv(gl::glGetUniformLocation(program, "u_light_dir"), 1, dir.dataPtr());
			
			main_scene.cycle();
			// rear_turret->transform_priority(midnight::matrixRotation(midnight::Vector3{0, 1, 0}, 0.1));
			// hull->transform_priority(midnight::matrixRotation(midnight::Vector3{0, 1, 0}, 0.1));

			glfwSwapBuffers(mw);
			glfwPollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds{100});
		}
	}

	glfwDestroyWindow(mw);
	glfwTerminate();
	return 0;
}
