#pragma once

#include <string>

#include <glm/glm.hpp>


std::string vec3ToStr(const float * vec);

std::string vec3ToStr(glm::vec3 vec);

std::string mat4ToStr(const float * mat);

std::string mat4ToStr(const float * mat, std::string separator);

std::string mat4ToStr(glm::mat4 mat);

std::string mat4ToStr(glm::mat4 mat, std::string separator);
