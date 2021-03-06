#version 440 core

layout (lines) in;
layout (triangle_strip, max_vertices = 70) out;

uniform float width;
uniform float height;

void drawCircle(float cx, float cy, float rx, float ry, int num_segments);

void main(){
	int numSegments = 32;

	float c1x = gl_in[0].gl_Position.x;
	float c1y = gl_in[0].gl_Position.y;
	float c2x = gl_in[1].gl_Position.x;
	float c2y = gl_in[1].gl_Position.y;
	
	//unnecessary to draw both circles, skipping one for efficiency
	
	//circle at inital cursor positon
	//drawCircle(c1x, c1y, width, height, numSegments);
	
	
	//circle at final cursor position
	drawCircle(c2x, c2y, width, height, numSegments);
	
	
	//draw the rectangle connecting both circles
	vec2 AB = vec2(c2x, c2y) - vec2(c1x, c1y);
	
	vec2 AK = normalize(vec2(-AB.y, AB.x));
	vec2 AC = vec2(AK.x * width, AK.y * height);
	
	vec2 C = vec2(c1x + AC.x, c1y + AC.y);
	vec2 D = vec2(c1x - AC.x, c1y - AC.y);
	vec2 E = vec2(c2x + AC.x, c2y + AC.y);
	vec2 F = vec2(c2x - AC.x, c2y - AC.y);
	
	//gl_Position = vec4(D.xy, 0.0, 1.0);
	gl_Position = vec4(c1x - AC.x, c1y - AC.y, 0.0, 1.0);
	EmitVertex();
	//gl_Position = vec4(C.xy, 0.0, 1.0);
	gl_Position = vec4(c1x + AC.x, c1y + AC.y, 0.0, 1.0);
	EmitVertex();
	//gl_Position = vec4(F.xy, 0.0, 1.0);
	gl_Position = vec4(c2x - AC.x, c2y - AC.y, 0.0, 1.0);
	EmitVertex();
	//gl_Position = vec4(E.xy, 0.0, 1.0);
	gl_Position = vec4(c2x + AC.x, c2y + AC.y, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();

}

//it's actually an ellipse but shut up
void drawCircle(float cx, float cy, float rx, float ry, int num_segments){
	float theta = 2 * 3.1415926 / num_segments;
	float tan_factor = tan(theta);
	float rad_factor = cos(theta);
	
	float x = 1;
	float y = 0;
	
	for(int i = 0; i <= num_segments; i++){
		//edge point
		gl_Position = vec4(x * rx + cx, y * ry + cy, 0.0, 1.0);
		EmitVertex();
		
		//center point
		gl_Position = vec4(cx, cy, 0.0, 1.0);
		EmitVertex();
		
		//tangential vector
		float tx = -y;
		float ty = x;
		
		//scale tangential vector
		x += tx * tan_factor;
		y += ty * tan_factor;
		
		//get correct position
		x *= rad_factor;
		y *= rad_factor;
		
	}
	EndPrimitive();
}
