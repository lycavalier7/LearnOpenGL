#pragma once

#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	uint32_t programId;

	Shader(const char* pVertexPath, const char* pFragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			std::stringstream vShaderString;
			std::stringstream fShaderString;

			vShaderFile.open(pVertexPath);
			fShaderFile.open(pFragmentPath);

			vShaderString << vShaderFile.rdbuf();
			fShaderString << fShaderFile.rdbuf();

			vertexCode = vShaderString.str();
			fragmentCode = fShaderString.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}

		const char* pVertexSource = vertexCode.c_str();
		const char* pFragmentSource = fragmentCode.c_str();

		uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &pVertexSource, NULL);
		glCompileShader(vertex);
		CheckProgramError(vertex, ProgramType::Shader);

		uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &pFragmentSource, NULL);
		glCompileShader(fragment);
		CheckProgramError(fragment, ProgramType::Shader);

		programId = glCreateProgram();
		glAttachShader(programId, vertex);
		glAttachShader(programId, fragment);
		glLinkProgram(programId);
		CheckProgramError(programId, ProgramType::Program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Use()
	{
		glUseProgram(programId);
	}

	void SetInt(std::string uniformName, int32_t value)
	{
		uint32_t loction = glGetUniformLocation(programId, uniformName.c_str());
		glUniform1i(loction, value);
	}

	void SetMat4(std::string uniformName, float* value)
	{
		uint32_t location = glGetUniformLocation(programId, uniformName.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
	}

private:
	enum class ProgramType
	{
		Shader = 0,
		Program
	};

	void CheckProgramError(uint32_t shader, ProgramType type)
	{
		int32_t success;
		char infoLog[512];

		if (type == ProgramType::Shader)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (success == 0)
			{
				std::string shaderType = "SHADER";

				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);

			if (success == 0)
			{
				std::string shaderType = "PROGRAM";

				glGetProgramInfoLog(shader, 512, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << shaderType << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};