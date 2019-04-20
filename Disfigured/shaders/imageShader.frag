#version 440 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D image;

void main(){
	FragColor = texture(image, texCoords);
}
