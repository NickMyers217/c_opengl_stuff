#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    //fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	fragColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.4);
} 