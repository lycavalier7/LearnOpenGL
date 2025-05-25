#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <stdint.h>

uint32_t WindowsWidth = 1920;
uint32_t WindowsHeight = 1080;

const GLchar* pVertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"out vec4 outPosition;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   outPosition = vec4(aPos, 1.0);\n"
"}\n\0";

const GLchar* pFragmentShaderSource = "#version 330 core\n"
"in vec4 outPosition;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = outPosition;\n"
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
	glShaderSource(vertexShader, 1, &pVertexShaderSource, NULL);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (success == false)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &pFragmentShaderSource, NULL);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (success == false)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (success == false)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGREAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteProgram(vertexShader);
	glDeleteProgram(fragmentShader);

	float vertices[] = {
	-0.5f, -0.5f, 0.0f, // left  
	 0.5f, -0.5f, 0.0f, // right 
	 0.0f,  0.5f, 0.0f  // top   
	};

	uint32_t VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glfwSetFramebufferSizeCallback(pWindow, FrameSizeCallBack);

	while (glfwWindowShouldClose(pWindow) == false)
	{
		glClearColor(0.2, 0.3, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ProcessInput(pWindow);
		glfwPollEvents();

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(pWindow);
	}

	glfwTerminate();
	return 0;
}