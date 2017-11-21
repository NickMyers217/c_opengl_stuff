#pragma once

#include <stdio.h>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"


const std::string SHADER_PROGRAM_PATH = "../OpenGLTesting/res/shaders/";


class ShaderProgram {
public:
	GLuint Id;

	ShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource);
	~ShaderProgram();

	void Use();
	void SetUniform(const char * uniform, int value);
	void SetUniform(const char * uniform, float value);
	void SetUniform(const char * uniform, glm::vec3& value);
	void SetUniform(const char * uniform, glm::mat4& value);

private:
	void CompileShader(GLuint id, const char * shaderSource);
};


ShaderProgram::ShaderProgram(const char * vertexShaderName, const char * fragmentShaderName)
{
	std::string vertexShaderPath = SHADER_PROGRAM_PATH + vertexShaderName;
	std::string fragmentShaderPath = SHADER_PROGRAM_PATH + fragmentShaderName;
	std::string vertexShaderSource = readFileToString(vertexShaderPath.c_str());
	std::string fragmentShaderSource = readFileToString(fragmentShaderPath.c_str());

	GLuint vertId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragId = glCreateShader(GL_FRAGMENT_SHADER);

	CompileShader(vertId, vertexShaderSource.c_str());
	CompileShader(fragId, fragmentShaderSource.c_str());

	Id = glCreateProgram();
	int  success;
	char infoLog[512];
	glAttachShader(Id, vertId);
	glAttachShader(Id, fragId);
	glLinkProgram(Id);
	glGetProgramiv(Id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Id, 512, NULL, infoLog);
		printf("ERROR::SHADER::LINKING_FAILED\n%s\n", infoLog);
	}
	glDeleteShader(vertId);
	glDeleteShader(fragId);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(Id);
}

void ShaderProgram::CompileShader(GLuint id, const char * shaderSource)
{
	int  success;
	char infoLog[512];
	glShaderSource(id, 1, &shaderSource, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
	}
}

void ShaderProgram::Use()
{
	glUseProgram(Id);
}

inline void ShaderProgram::SetUniform(const char * uniform, int value)
{
	glUniform1i(glGetUniformLocation(Id, uniform), value);
}

inline void ShaderProgram::SetUniform(const char * uniform, float value)
{
	glUniform1f(glGetUniformLocation(Id, uniform), value);
}

inline void ShaderProgram::SetUniform(const char * uniform, glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(Id, uniform), value.x, value.y, value.z);
}

inline void ShaderProgram::SetUniform(const char * uniform, glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(Id, uniform), 1, GL_FALSE, glm::value_ptr(value));
}
