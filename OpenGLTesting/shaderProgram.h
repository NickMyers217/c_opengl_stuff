#pragma once

#include <stdio.h>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "utils.h"


struct ShaderProgram {
	GLuint id;
};

const std::string SHADER_PROGRAM_PATH = "../OpenGLTesting/res/shaders/";


void compileShader(GLuint id, const char * shaderSource);

void shaderProgramBuild(ShaderProgram * program, const char * vertexShaderSource, const char * fragmentShaderSource);
void shaderProgramUse(ShaderProgram * program);
void shaderProgramFree(ShaderProgram * program);
void shaderProgramSet(ShaderProgram * program, const char * uniform, int value);
void shaderProgramSet(ShaderProgram * program, const char * uniform, float value);
void shaderProgramSet(ShaderProgram * program, const char * uniform, glm::vec3& value);
void shaderProgramSet(ShaderProgram * program, const char * uniform, glm::mat4& value);


void compileShader(GLuint id, const char * shaderSource)
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

void shaderProgramBuild(ShaderProgram * program, const char * vertexShaderName, const char * fragmentShaderName)
{
	std::string vertexShaderPath = SHADER_PROGRAM_PATH + vertexShaderName;
	std::string fragmentShaderPath = SHADER_PROGRAM_PATH + fragmentShaderName;
	std::string vertexShaderSource = readFileToString(vertexShaderPath.c_str());
	std::string fragmentShaderSource = readFileToString(fragmentShaderPath.c_str());

	GLuint vertId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragId = glCreateShader(GL_FRAGMENT_SHADER);

	compileShader(vertId, vertexShaderSource.c_str());
	compileShader(fragId, fragmentShaderSource.c_str());

	program->id = glCreateProgram();
	int  success;
	char infoLog[512];
	glAttachShader(program->id, vertId);
	glAttachShader(program->id, fragId);
	glLinkProgram(program->id);
	glGetProgramiv(program->id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program->id, 512, NULL, infoLog);
		printf("ERROR::SHADER::LINKING_FAILED\n%s\n", infoLog);
	}
	glDeleteShader(vertId);
	glDeleteShader(fragId);
}

void shaderProgramUse(ShaderProgram * program)
{
	glUseProgram(program->id);
}

void shaderProgramFree(ShaderProgram * program)
{
	glDeleteProgram(program->id);
}

inline void shaderProgramSet(ShaderProgram * program, const char * uniform, int value)
{
	glUniform1i(glGetUniformLocation(program->id, uniform), value);
}

inline void shaderProgramSet(ShaderProgram * program, const char * uniform, float value)
{
	glUniform1f(glGetUniformLocation(program->id, uniform), value);
}

inline void shaderProgramSet(ShaderProgram * program, const char * uniform, glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(program->id, uniform), value.x, value.y, value.z);
}

inline void shaderProgramSet(ShaderProgram * program, const char * uniform, glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(program->id, uniform), 1, GL_FALSE, glm::value_ptr(value));
}
