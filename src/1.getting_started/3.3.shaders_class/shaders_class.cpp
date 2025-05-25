#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "learnopengl/shader_s.h"

#include <iostream>
#include <stdint.h>

uint32_t WindowsWidth = 1920;
uint32_t WindowsHeight = 1080;


void FrameSizeCallBack(GLFWwindow* pWindow, int32_t width, int32_t height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* pWindow)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(WindowsWidth, WindowsHeight, "Hello Triangle", NULL, NULL);

	if (pWindow == nullptr)
	{
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(pWindow);

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
	{
		std::cout << "Fail to load Opengl proc address" << std::endl;
		glfwTerminate();

		return -1;
	}


	// Build program and compile shader
	Shader shader("3.3.shader.vs", "3.3.shader.fs");

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

	};

	uint32_t VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glfwSetFramebufferSizeCallback(pWindow, FrameSizeCallBack);

	while (glfwWindowShouldClose(pWindow) == false)
	{
		glClearColor(0.2, 0.3, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ProcessInput(pWindow);
		glfwPollEvents();
	
		shader.Use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(pWindow);
	}

	glfwTerminate();
	return 0;
}