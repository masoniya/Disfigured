#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "ToString.h"


std::string vec3ToStr(const float * vec)
{
	glm::vec3 vector = glm::make_vec3(vec);
	return vec3ToStr(vector);
}

std::string vec3ToStr(glm::vec3 vec)
{
	std::ostringstream out;
	out << "{ " << vec[0] << ", " << vec[1] << ", " << vec[2] << " }";
	return out.str();
}

std::string mat4ToStr(const float * mat)
{
	return mat4ToStr(mat, ", ");
}

std::string mat4ToStr(const float * mat, std::string separator)
{
	glm::mat4 matrix = glm::make_mat4(mat);
	return mat4ToStr(glm::transpose(matrix));
}

std::string mat4ToStr(glm::mat4 mat) 
{
	return mat4ToStr(mat, ", ");
}

std::string mat4ToStr(glm::mat4 mat, std::string separator)
{
	std::ostringstream out;
	out << "{ { " << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", " << mat[0][3] << " }";
	out << separator << "{ " << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", " << mat[1][3] << " }";
	out << separator << "{ " << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ", " << mat[2][3] << " }";
	out << separator << "{ " << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2] << ", " << mat[3][3] << " } }";
	return out.str();
}
