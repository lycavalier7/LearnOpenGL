#include"glad/glad.h"
#include"GLFW/glfw3.h"

#include <stdint.h>
#include <iostream>

uint32_t WindowsWidth  = 1920;
uint32_t WindowsHeight = 1080;

void FrameSizeCallBack(GLFWwindow* pWindows, int32_t width, int32_t height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* pWindows)
{
	if (glfwGetKey(pWindows, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindows, true);
	}
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindows = glfwCreateWindow(WindowsWidth, WindowsHeight, "Hello Window", NULL, NULL);

	if (pWindows == nullptr)
	{
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(pWindows);

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
	{
		std::cout << "Failed to load OpenGL proc address" << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwSetFramebufferSizeCallback(pWindows, FrameSizeCallBack);

	while (glfwWindowShouldClose(pWindows) == false)
	{
		ProcessInput(pWindows);
		glfwPollEvents();
		glfwSwapBuffers(pWindows);

		glClearColor(0.2, 0.3, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	glfwTerminate();

	return 0;
}