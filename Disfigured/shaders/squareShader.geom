#version 440 core

layout (lines) in;
layout (triangle_strip, max_vertices = 16) out;

uniform float width;
uniform float height;

void main(){
	float x1 = gl_in[0].gl_Position.x;
	float y1 = gl_in[0].gl_Position.y;
	float x2 = gl_in[1].gl_Position.x;
	float y2 = gl_in[1].gl_Position.y;
	
	//unecessary to draw both squares but doing it anyway
	
	//square at the initial cursor position
	//2
	gl_Position = vec4(x1 - width, y1 + height, 0.0, 1.0);
	EmitVertex();
	//1
	gl_Position = vec4(x1 + width, y1 + height, 0.0, 1.0);
	EmitVertex();
	//3
	gl_Position = vec4(x1 - width, y1 - height, 0.0, 1.0);
	EmitVertex();
	//4
	gl_Position = vec4(x1 + width, y1 - height, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
	
	//square at the final cursor destination
	//6
	gl_Position = vec4(x2 - width, y2 + height, 0.0, 1.0);
	EmitVertex();
	//5
	gl_Position = vec4(x2 + width, y2 + height, 0.0, 1.0);
	EmitVertex();
	//7
	gl_Position = vec4(x2 - width, y2 - height, 0.0, 1.0);
	EmitVertex();
	//8
	gl_Position = vec4(x2 + width, y2 - height, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
	
	//rectangle between squares
	//3
	gl_Position = vec4(x1 - width, y1 - height, 0.0, 1.0);
	EmitVertex();
	//1
	gl_Position = vec4(x1 + width, y1 + height, 0.0, 1.0);
	EmitVertex();
	//7
	gl_Position = vec4(x2 - width, y2 - height, 0.0, 1.0);
	EmitVertex();
	//5
	gl_Position = vec4(x2 + width, y2 + height, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
	
	//another rectangle from the other corners
	//2
	gl_Position = vec4(x1 - width, y1 + height, 0.0, 1.0);
	EmitVertex();
	//4
	gl_Position = vec4(x1 + width, y1 - height, 0.0, 1.0);
	EmitVertex();
	//6
	gl_Position = vec4(x2 - width, y2 + height, 0.0, 1.0);
	EmitVertex();
	//8
	gl_Position = vec4(x2 + width, y2 - height, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
	
}
