#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform mat4 view;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(view * vec4(lightPos, 1.0)) - fragPos);
	float diffuseImpact = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseImpact * lightColor;

	// specular lighting
	float specularStrength = 0.5;
	vec3 viewPos = vec3(0.0); // its the origin since we are doing this calculation in view space
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	vec3 specular = specularStrength * spec * lightColor;


	vec4 objectColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.4);

	fragColor = vec4((ambient + diffuse + specular) * objectColor.xyz, 1.0);
} 
