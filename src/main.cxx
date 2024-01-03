#include <cstddef>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	GLFWwindow *mw{glfwCreateWindow(800, 600, "The Amaranthine Project", NULL, NULL)};
	glfwMakeContextCurrent(mw);
	const int gll{gladLoadGL(glfwGetProcAddress)};

	while (!glfwWindowShouldClose(mw)) {

	}

	glfwDestroyWindow(mw);
	glfwTerminate();
	return 0;
}
