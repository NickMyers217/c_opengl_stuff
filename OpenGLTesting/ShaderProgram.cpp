#include "ShaderProgram.h"

const std::string SHADER_PROGRAM_PATH = "../OpenGLTesting/res/shaders/";

ShaderProgram::ShaderProgram(const char * vertexShaderName, const char * fragmentShaderName)
{
	std::string vertexShaderPath = SHADER_PROGRAM_PATH + vertexShaderName;
	std::string fragmentShaderPath = SHADER_PROGRAM_PATH + fragmentShaderName;
	std::string vertexShaderSource = MyUtils::readFileToString(vertexShaderPath.c_str());
	std::string fragmentShaderSource = MyUtils::readFileToString(fragmentShaderPath.c_str());

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

void ShaderProgram::SetUniform(const char * uniform, const int value) const
{
	glUniform1i(glGetUniformLocation(Id, uniform), value);
}

void ShaderProgram::SetUniform(const char * uniform, const float value) const
{
	glUniform1f(glGetUniformLocation(Id, uniform), value);
}

void ShaderProgram::SetUniform(const char * uniform, const glm::vec3& value) const
{
	glUniform3f(glGetUniformLocation(Id, uniform), value.x, value.y, value.z);
}

void ShaderProgram::SetUniform(const char * uniform, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(Id, uniform), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const char * uniform, const Light& light) const
{
	std::string object = uniform;
	switch (light.Type)
	{
	case LightType::POINT_LIGHT:
		SetUniform((object + ".position").c_str(), light.Position);
		SetUniform((object + ".color").c_str(), light.Color);
		SetUniform((object + ".ambient").c_str(), light.Ambient);
		SetUniform((object + ".diffuse").c_str(), light.Diffuse);
		SetUniform((object + ".specular").c_str(), light.Specular);
		SetUniform((object + ".constant").c_str(), light.Constant);
		SetUniform((object + ".linear").c_str(), light.Linear);
		SetUniform((object + ".quadratic").c_str(), light.Quadratic);
		break;
	case LightType::DIRECTIONAL_LIGHT:
		SetUniform((object + ".direction").c_str(), light.Direction);
		SetUniform((object + ".color").c_str(), light.Color);
		SetUniform((object + ".ambient").c_str(), light.Ambient);
		SetUniform((object + ".diffuse").c_str(), light.Diffuse);
		SetUniform((object + ".specular").c_str(), light.Specular);
		break;
	case LightType::SPOT_LIGHT:
		SetUniform((object + ".position").c_str(), light.Position);
		SetUniform((object + ".direction").c_str(), light.Direction);
		SetUniform((object + ".color").c_str(), light.Color);
		SetUniform((object + ".ambient").c_str(), light.Ambient);
		SetUniform((object + ".diffuse").c_str(), light.Diffuse);
		SetUniform((object + ".specular").c_str(), light.Specular);
		SetUniform((object + ".constant").c_str(), light.Constant);
		SetUniform((object + ".linear").c_str(), light.Linear);
		SetUniform((object + ".quadratic").c_str(), light.Quadratic);
		SetUniform((object + ".cutOff").c_str(), light.CutOff);
		SetUniform((object + ".outerCutOff").c_str(), light.OuterCutOff);
		break;
	}
}

void ShaderProgram::SetUniform(const char * uniform, const std::vector<Light>& lights) const
{
	std::string object = uniform;
	for (unsigned int i = 0; i < lights.size(); i++)
		SetUniform((object + "[" + std::to_string(i) + "]").c_str(), lights[i]);
}
