#pragma once

#include <glm/glm.hpp>


//ranges from 0.0f to 1.0f for all values

void rgb_to_hsl(glm::vec3 rgb, float * h, float * s, float * l);
void rgb_to_hsl(float r, float g, float b, float * h, float * s, float * l);

void hsl_to_rgb(glm::vec3 hsl, float * r, float * g, float * b);
void hsl_to_rgb(float h, float s, float l, float * r, float * g, float * b);
