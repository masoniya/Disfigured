#include "ColorTransform.h"


void rgb_to_hsl(glm::vec3 rgb, float * h, float * s, float * l)
{
	rgb_to_hsl(rgb.r, rgb.g, rgb.b, h, s, l);
}

void rgb_to_hsl(float r, float g, float b, float * h, float * s, float * l)
{
	float min, max;

	min = r;
	if (min > g) min = g;
	if (min > b) min = b;

	max = r;
	if (max < g) max = g;
	if (max < b) max = b;

	*l = (min + max) / 2;
	
	if (*l < 0.5f) {
		*s = (max - min) / (max + min);
	}
	else {
		*s = (max - min) / (2.0f - max - min);
	}

	if (max - r < 0.0000001f) {
		*h = (g - b) / (max - min);
	}
	else if (max - g < 0.0000001f) {
		*h = 2.0f + (b - r) / (max - min);
	}
	else if (max - b < 0.0000001f) {
		*h = 4.0f + (r - g) / (max - min);
	}

	*h /= 6.0f;

	if (*h < 0) {
		*h += 1.0f;
	}
}

void hsl_to_rgb(glm::vec3 hsl, float * r, float * g, float * b)
{
	hsl_to_rgb(hsl.r, hsl.g, hsl.b, r, g, b);
}

void hsl_to_rgb(float h, float s, float l, float * r, float * g, float * b)
{
	if (s < 0.0000001f) {
		*r = l;
		*g = l;
		*b = l;
		return;
	}

	float temp1, temp2, tempR, tempG, tempB;

	if (l < 0.5f) {
		temp1 = l * (1.0f + s);
	}
	else {
		temp1 = l + s - l * s;
	}

	temp2 = 2 * l - temp1;

	tempR = h + 0.333f;
	tempG = h;
	tempB = h - 0.333f;

	//calculate red
	while (tempR > 1.0f) {
		tempR -= 1.0f;
	}
	while (tempR < 0.0f) {
		tempR += 1.0f;
	}

	if (6 * tempR < 1.0f) {
		*r = temp2 + (temp1 - temp2) * 6 * tempR;
	}
	else if (2 * tempR < 1.0f) {
		*r = temp1;
	}
	else if (3 * tempR < 2.0f) {
		*r = temp2 + (temp1 - temp2) * (0.666f - tempR) * 6;
	}
	else {
		*r = temp2;
	}

	//calculate green
	while (tempG > 1.0f) {
		tempG -= 1.0f;
	}
	while (tempG < 0.0f) {
		tempG += 1.0f;
	}

	if (6 * tempG < 1.0f) {
		*g = temp2 + (temp1 - temp2) * 6 * tempG;
	}
	else if (2 * tempG < 1.0f) {
		*g = temp1;
	}
	else if (3 * tempG < 2.0f) {
		*g = temp2 + (temp1 - temp2) * (0.666f - tempG) * 6;
	}
	else {
		*g = temp2;
	}

	//calculate blue
	while (tempB > 1.0f) {
		tempB -= 1.0f;
	}
	while (tempB < 0.0f) {
		tempB += 1.0f;
	}

	if (6 * tempB < 1.0f) {
		*b = temp2 + (temp1 - temp2) * 6 * tempB;
	}
	else if (2 * tempB < 1.0f) {
		*b = temp1;
	}
	else if (3 * tempB < 2.0f) {
		*b = temp2 + (temp1 - temp2) * (0.666f - tempB) * 6;
	}
	else {
		*b = temp2;
	}
}
