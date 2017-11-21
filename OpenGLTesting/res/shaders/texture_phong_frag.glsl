#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
  vec3 diffuse;
  vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
//uniform DirectionalLight light;
uniform PointLight light;
uniform mat4 view;

void main()
{
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	// diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(view * vec4(light.position, 1.0)) - fragPos);
	//vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	// specular lighting
	vec3 viewPos = vec3(0.0); // its the origin since we are doing this calculation in view space
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

	// attenuation
	float distance = length(vec3(view * vec4(light.position, 1.0)) - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	fragColor = vec4(ambient + diffuse + specular, 1.0);
} 
