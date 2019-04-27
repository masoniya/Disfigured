#version 440 core

out vec4 FragColor;

uniform vec3 color;
uniform float seed;


float rand(vec2 co);

void main(){
	vec2 seedVec = seed * gl_FragCoord.xy;
	if(rand(seedVec) > 0.04){
		discard;
	}
	
	FragColor = vec4(color, 1.0);
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
