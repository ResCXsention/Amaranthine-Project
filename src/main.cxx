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

#include <midnight/matrix.hxx>
#include <midnight/polar.hxx>
#include <midnight/util.hxx>

namespace glfwcbs
{
	void framebufferSizeCb(GLFWwindow *win, int w, int h)
	{
		gl::glViewport(0, 0, w, h);
	}
}

// #include <array>
// #include <vector>
// #include <numbers>
// #include <midnight/matrix.hxx>
// #include <midnight/polar.hxx>
// #include <midnight/util.hxx>
std::array<unsigned int, 4> *sphericalObject(
		const float radius,
		const unsigned int longitudes,
		const unsigned int latitudes)
{
	assert(radius > 0);
	assert(longitudes > 0);
	assert(latitudes > 0);

	const float lon_step{(2 * std::numbers::pi_v<float>) / longitudes};
	const float half_lon_step{lon_step / 2};
	const float lat_step{std::numbers::pi_v<float> / (latitudes + 1)};
	const float depression_to_point_up{-std::numbers::pi_v<float> / 2};

	std::vector<float> points;
	points.reserve((longitudes * latitudes + 2) * 3);
	// top point
	points.insert(points.begin(), {0, radius, 0});
	// formation points
	for (int lat{0}; lat < latitudes; ++lat) {
		const float current_lat{lat_step * (lat + 1)};
		for (int lon{0}; lon < longitudes; ++lon) {
			const midnight::Polar t{radius, lon * lon_step, depression_to_point_up + current_lat};
			midnight::Vector3 c{midnight::cartesian3(t)};
			if (std::fabs(c.entry(0, 0)) < 0.00001F) {
				c.entry(0, 0) = 0.0F;
			}
			if (std::fabs(c.entry(1, 0)) < 0.00001F) {
				c.entry(1, 0) = 0.0F;
			}
			if (std::fabs(c.entry(2, 0)) < 0.00001F) {
				c.entry(2, 0) = 0.0F;
			}
			points.push_back(c.entry(0, 0));
			points.push_back(c.entry(1, 0));
			points.push_back(c.entry(2, 0));
		}
	}
	// bottom point
	points.insert(points.end(), {0, -radius, 0});

	std::vector<unsigned int> indices;
	indices.reserve(longitudes * latitudes * 6);
	// connect top to first latitude
	for (int lon{0}; lon < longitudes; ++lon) {
		indices.push_back(0);
		indices.push_back(1 + lon);
		indices.push_back(1 + (lon != longitudes - 1 ? lon + 1 : 0));
	}
	// connect latitudes to each other
	if (latitudes > 1) {
		for (int lat{0}; lat < latitudes - 1; ++lat) {
			const unsigned int lat_offset{lat * longitudes};
			const unsigned int next_lat_offset{(1 + lat) * longitudes};
			for (int lon{0}; lon < longitudes; ++lon) {
				const int lon_wrap{lon != longitudes - 1 ? lon + 1 : 0};
				// first triangle
				indices.push_back(1 + lat_offset + lon);
				indices.push_back(1 + next_lat_offset + lon);
				indices.push_back(1 + next_lat_offset + lon_wrap);
				// second triangle to complete the rectangle
				indices.push_back(1 + lat_offset + lon);
				indices.push_back(1 + next_lat_offset + lon_wrap);
				indices.push_back(1 + lat_offset + lon_wrap);
			}
		}
	}
	// connect bottom to last latitude
	// the bottom point is defined as longitudes * latitudes + 1
	for (int lon{0}; lon < longitudes; ++lon) {
		indices.push_back(1 + (latitudes - 1) * longitudes + lon);
		indices.push_back(1 + longitudes * latitudes);
		indices.push_back(1 + (latitudes - 1) * longitudes + (lon != longitudes - 1 ? lon + 1 : 0));
	}
	
	std::vector<float> normals;
	normals.reserve((longitudes * latitudes + 2) * 3);
	std::vector<std::vector<unsigned int>> point_triangles(longitudes * latitudes + 2);
	point_triangles.front().reserve(longitudes * 3);
	point_triangles.back().reserve(longitudes * 3);
	for (auto t{point_triangles.begin() + 1}; t != point_triangles.end() - 1; ++t) {
		t->reserve(15);
	}
	// index triangles
	for (auto t{indices.begin()}; t != indices.end(); t += 3) {
		const unsigned int a{*t}, b{*(t + 1)}, c{*(t + 2)};
		point_triangles.at(a).push_back(a);
		point_triangles.at(a).push_back(b);
		point_triangles.at(a).push_back(c);
		point_triangles.at(b).push_back(a);
		point_triangles.at(b).push_back(b);
		point_triangles.at(b).push_back(c);
		point_triangles.at(c).push_back(a);
		point_triangles.at(c).push_back(b);
		point_triangles.at(c).push_back(c);
	}
	// compute normals
	for (int p{0}; p < longitudes * latitudes + 2; ++p) {
		midnight::Vector3 n;
		for (auto t{point_triangles.at(p).begin()}; t != point_triangles.at(p).end(); t += 3) {
			const unsigned int a{*t}, b{*(t + 1)}, c{*(t + 2)};
			const midnight::Vector3 v1{points.at(a * 3), points.at(a * 3 + 1), points.at(a * 3 + 2)};
			const midnight::Vector3 v2{points.at(b * 3), points.at(b * 3 + 1), points.at(b * 3 + 2)};
			const midnight::Vector3 v3{points.at(c * 3), points.at(c * 3 + 1), points.at(c * 3 + 2)};
			n += midnight::cross(v1 - v3, v2 - v3);
		}
		n = midnight::normalise(n);
		normals.push_back(n.entry(0, 0));
		normals.push_back(n.entry(1, 0));
		normals.push_back(n.entry(2, 0));
	}
	
	auto *mod{new std::array<unsigned int, 4>}; // vao, vbo, ibo, normals
	gl::glGenVertexArrays(1, &mod->at(0));
	gl::glGenBuffers(3, &mod->at(1));
	gl::glBindVertexArray(mod->at(0));

	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, mod->at(1));
	gl::glBufferData(gl::GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), gl::GL_STATIC_DRAW);
	gl::glEnableVertexAttribArray(0);
	gl::glVertexAttribPointer(0, 3, gl::GL_FLOAT, false, 0, reinterpret_cast<void*>(0));

	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, mod->at(3));
	gl::glBufferData(gl::GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), gl::GL_STATIC_DRAW);
	gl::glEnableVertexAttribArray(1);
	gl::glVertexAttribPointer(1, 3, gl::GL_FLOAT, false, 0, reinterpret_cast<void*>(0));

	gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, mod->at(2));
	gl::glBufferData(gl::GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), gl::GL_STATIC_DRAW);

	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);
	gl::glBindVertexArray(0);
	gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, 0);

	return mod;
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

	const double points[9]{
		-200.0F, -200.0F, 0.0F,
		200.0F, -200.0F, 0.0F,
		0.0F, 200.0F, 0.0F
	};
	unsigned int vbo{0}, vao{0};
	gl::glGenBuffers(1, &vbo);
	gl::glGenVertexArrays(1, &vao);

	gl::glBindVertexArray(vao);
	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, vbo);
	gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(points), points, gl::GL_STATIC_DRAW);
	gl::glVertexAttribPointer(0, 3, gl::GL_DOUBLE, false, 3 * sizeof(float), reinterpret_cast<void*>(0));
	gl::glEnableVertexAttribArray(0);

	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);
	gl::glBindVertexArray(0);

	const char *vertex_source;
	const char *fragment_source;
	try {
		vertex_source = midnight::readFile("../src/vertex.glsl");
		fragment_source = midnight::readFile("../src/fragment.glsl");
	} catch (const std::ios_base::failure &e) {
		std::cout << "exception, attempt to read a nonexistent file: " << e.what() << std::endl;
		std::terminate();
	}
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

	std::array<unsigned int, 4> *sphere{sphericalObject(1, 4, 1)};

	while (!glfwWindowShouldClose(mw)) {
		gl::glClear(gl::GL_COLOR_BUFFER_BIT);

		gl::glBindVertexArray(sphere->at(0));
		gl::glUseProgram(program);
		gl::glDrawElements(gl::GL_TRIANGLES, 24, gl::GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

		const float t{static_cast<const float>(glfwGetTime())};
		midnight::Vector3 dir{midnight::cartesian3({1, std::cos(t), std::sin(t)})};
		gl::glUniform3fv(gl::glGetUniformLocation(program, "u_light_dir"), 1, dir.dataPtr());
		m *= midnight::matrixRotation(dir, 0.05);
		gl::glUniformMatrix4fv(gl::glGetUniformLocation(program, "u_model"), 1, false, m.dataPtr());

		glfwSwapBuffers(mw);
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds{100});
	}

	glfwDestroyWindow(mw);
	glfwTerminate();
	return 0;
}
