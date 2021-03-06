#version 440 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D textImage;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textImage, texCoords).r);
	FragColor = vec4(textColor, 1.0) * sampled;
}
