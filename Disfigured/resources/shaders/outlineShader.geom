#version 440 core

layout (lines) in;
layout (line_strip, max_vertices = 5) out;

void main(){
	float x1 = gl_in[0].gl_Position.x;
	float y1 = gl_in[0].gl_Position.y;
	float x2 = gl_in[1].gl_Position.x;
	float y2 = gl_in[1].gl_Position.y;
	
	gl_Position = vec4(x1, y1, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(x1, y2, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(x2, y2, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(x2, y1, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(x1, y1, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
}
