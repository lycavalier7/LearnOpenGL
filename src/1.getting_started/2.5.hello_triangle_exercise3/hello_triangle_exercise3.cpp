#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <stdint.h>

uint32_t WindowsWidth = 1920;
uint32_t WindowsHeight = 1080;

const GLchar* pVertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"}\n\0";

const GLchar* pFragmentShaderSource0 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n\0";

const GLchar* pFragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
"}\n\0";

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
	int success;
	char infoLog[512];

	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	uint32_t fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	uint32_t fragmentShaderBlue = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &pVertexShaderSource, NULL);
	glShaderSource(fragmentShaderRed, 1, &pFragmentShaderSource0, NULL);
	glShaderSource(fragmentShaderBlue, 1, &pFragmentShaderSource1, NULL);


	uint32_t shaderProgram0 = glCreateProgram();
	glAttachShader(shaderProgram0, vertexShader);
	glAttachShader(shaderProgram0, fragmentShaderRed);
	glLinkProgram(shaderProgram0);

	uint32_t shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShaderBlue);
	glLinkProgram(shaderProgram1);

	glDeleteProgram(vertexShader);
	glDeleteProgram(fragmentShaderRed);
	glDeleteProgram(fragmentShaderBlue);

	float firstTriangle[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};


	uint32_t VAO[2], VBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glfwSetFramebufferSizeCallback(pWindow, FrameSizeCallBack);

	while (glfwWindowShouldClose(pWindow) == false)
	{
		glClearColor(0.2, 0.3, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ProcessInput(pWindow);
		glfwPollEvents();

		glUseProgram(shaderProgram0);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram1);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(pWindow);
	}

	glfwTerminate();
	return 0;
}