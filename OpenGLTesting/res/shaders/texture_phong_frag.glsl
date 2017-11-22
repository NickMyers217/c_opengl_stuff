#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

// TODO: find a way to cut out some of the code duplication across the light structs
struct PointLight {
	vec3 position;

	vec3 color;
	vec3 ambient;
  vec3 diffuse;
  vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 color;
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

uniform mat4 view;
uniform Material material;
uniform DirectionalLight dirLight;
#define NR_POINT_LIGHTS 5
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	// view space light direction
	vec3 lightDir = normalize(-vec3(view * vec4(light.direction, 0.0)));

	// diffuse lighting
	float diff = max(dot(normal, lightDir), 0.0);

	// specular lighting
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, texCoord).rgb;
	vec3 specular = light.specular * spec * texture(material.texture_specular1, texCoord).rgb;

	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	// view space light direction
	vec3 lightDir = normalize(vec3(view * vec4(light.position, 1.0)) - fragPos);

	// diffuse lighting
	float diff = max(dot(normal, lightDir), 0.0);

	// specular lighting
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord)) * light.color;
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, texCoord).rgb * light.color;
	vec3 specular = light.specular * spec * texture(material.texture_specular1, texCoord).rgb * light.color;

	// attenuation
	float distance = length(vec3(view * vec4(light.position, 1.0)) - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	// view space light direction
	vec3 lightDir = normalize(vec3(view * vec4(light.position, 1.0)) - fragPos);

	// diffuse lighting
	float diff = max(dot(normal, lightDir), 0.0);

	// specular lighting
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord)) * light.color;
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, texCoord).rgb * light.color;
	vec3 specular = light.specular * spec * texture(material.texture_specular1, texCoord).rgb * light.color;

	// spot light
	float theta = dot(lightDir, normalize(-vec3(view * vec4(light.direction, 0.0))));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	// attenuation
	float distance = length(vec3(view * vec4(light.position, 1.0)) - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

// Make sure to convert all of this from world to view space
void main()
{
	vec3 norm = normalize(normal);
	vec3 viewPos = vec3(0.0); // its the origin since we are doing this calculation in view space
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = calcDirectionalLight(dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += calcPointLight(pointLights[i], norm, viewDir);

	result += calcSpotLight(spotLight, norm, viewDir);

	fragColor = vec4(result, 1.0);
} 
