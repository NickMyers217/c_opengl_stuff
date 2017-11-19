#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diffuseImpact = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseImpact * lightColor;

	vec4 objectColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.4);

	fragColor = vec4((ambient + diffuse) * objectColor.xyz, 1.0);
} 
