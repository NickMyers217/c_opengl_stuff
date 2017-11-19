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

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform mat4 view;

void main()
{
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	// diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(view * vec4(light.position, 1.0)) - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	// specular lighting
	vec3 viewPos = vec3(0.0); // its the origin since we are doing this calculation in view space
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

	fragColor = vec4(ambient + diffuse + specular, 1.0);
} 
