#pragma once

#include <glm/glm.hpp>


enum class LightType {
	POINT_LIGHT,
	SPOT_LIGHT,
	DIRECTIONAL_LIGHT
};


struct Light {
	LightType Type;
	glm::vec3 Position, Direction;
	glm::vec3 Color, Ambient, Diffuse, Specular;
	float Constant, Linear, Quadratic;
	float CutOff, OuterCutOff;

	Light() {}

	static Light DirectionalLight(glm::vec3 direction,
		glm::vec3 color,
		glm::vec3 ambient = glm::vec3(0.2f),
		glm::vec3 diffuse = glm::vec3(0.2f),
		glm::vec3 specular = glm::vec3(0.1f))
	{
		Light light;
		light.Type = LightType::DIRECTIONAL_LIGHT;
		light.Direction = direction;
		light.Color = color;
		light.Ambient = ambient;
		light.Diffuse = diffuse;
		light.Specular = specular;
		return light;
	}

	static Light PointLight(glm::vec3 position,
		glm::vec3 color = glm::vec3(1.0f),
		glm::vec3 ambient = glm::vec3(0.1f),
		glm::vec3 diffuse = glm::vec3(0.4f),
		glm::vec3 specular = glm::vec3(1.0f),
		float constant = 1.0f,
		float linear = 0.09f,
		float quadratic = 0.032f)
	{
		Light light;
		light.Type = LightType::POINT_LIGHT;
		light.Position = position;
		light.Color = color;
		light.Ambient = ambient;
		light.Diffuse = diffuse;
		light.Specular = specular;
		light.Constant = constant;
		light.Linear = linear;
		light.Quadratic = quadratic;
		return light;
	}

	static Light SpotLight(glm::vec3 position,
		glm::vec3 direction,
		glm::vec3 color = glm::vec3(1.0f),
		glm::vec3 ambient = glm::vec3(0.3f),
		glm::vec3 diffuse = glm::vec3(0.5f),
		glm::vec3 specular = glm::vec3(1.0f),
		float constant = 1.0f,
		float linear = 0.09f,
		float quadratic = 0.032f,
		float cutOff = glm::cos(glm::radians(12.5f)),
		float outerCutOff = glm::cos(glm::radians(17.5f)))
	{
		Light light;
		light.Type = LightType::SPOT_LIGHT;
		light.Position = position;
		light.Direction = direction;
		light.Color = color;
		light.Ambient = ambient;
		light.Diffuse = diffuse;
		light.Specular = specular;
		light.Constant = constant;
		light.Linear = linear;
		light.Quadratic = quadratic;
		light.CutOff = cutOff;
		light.OuterCutOff = outerCutOff;
		return light;
	}
};

