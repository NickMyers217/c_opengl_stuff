#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	fragPos = vec3(view * model * vec4(aPos, 1.0));
    // Needs to be mat3(tranpose(inverse(view * model))) for non uniform scalings
	normal = mat3(view * model) * aNormal; 
	texCoord = aTexCoord;
}