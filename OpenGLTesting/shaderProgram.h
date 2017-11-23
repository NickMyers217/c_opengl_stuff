#pragma once

#include "utils.h"
#include "Light.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class ShaderProgram {
public:
	GLuint Id;

	ShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource);

	void Use();
	void SetUniform(const char * uniform, const int value) const;
	void SetUniform(const char * uniform, const float value) const;
	void SetUniform(const char * uniform, const glm::vec3& value) const;
	void SetUniform(const char * uniform, const glm::mat4& value) const;
	void SetUniform(const char * uniform, const Light& light) const;
	void SetUniform(const char * uniform, const std::vector<Light>& lights) const;

private:
	void CompileShader(GLuint id, const char * shaderSource);
};
