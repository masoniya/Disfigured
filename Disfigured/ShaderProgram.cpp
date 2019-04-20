#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>

#include "ShaderProgram.h"
#include "ToString.h"


//#define ULTRA_DEBUG_MODE


ShaderProgram::ShaderProgram(const char * vertPath, const char * fragPath)
{
	compileShaders(vertPath, fragPath);
}

ShaderProgram::ShaderProgram(const char * vertPath, const char * geomPath, const char * fragPath)
{
	compileShaders(vertPath, geomPath, fragPath);
}


void ShaderProgram::compileShaders(const char *vertPath, const char *fragPath)
{
	uint32_t vertexShader;
	uint32_t fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	compileShader(vertPath, vertexShader);
	compileShader(fragPath, fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::string errorMessage("Shader program linking failed");
		errorMessage += '\n';
		throw std::runtime_error(errorMessage);
	}

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void ShaderProgram::compileShaders(const char * vertPath, const char * geomPath, const char * fragPath)
{
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	compileShader(vertPath, vertexShader);
	compileShader(geomPath, geometryShader);
	compileShader(fragPath, fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::string errorMessage("Shader program linking failed");
		errorMessage += '\n';
		throw std::runtime_error(errorMessage);
	}

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, geometryShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

void ShaderProgram::use()
{
	glUseProgram(shaderProgram);
}

void ShaderProgram::setUniformInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(shaderProgram, name), value);

#ifdef ULTRA_DEBUG_MODE
	std::cout << "Assigning Integer " << name << " with the value : " << value << std::endl;
#endif
}

void ShaderProgram::setUniformInt(std::string name, int value)
{
	setUniformInt(name.c_str(), value);
}

void ShaderProgram::setUniformFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, name), value);

#ifdef ULTRA_DEBUG_MODE
	std::cout << "Assigning Float " << name << " with the value : " << value << std::endl;
#endif
}

void ShaderProgram::setUniformFloat(std::string name, float value)
{
	setUniformFloat(name.c_str(), value);
}

void ShaderProgram::setUniformVec3(const char* name, glm::vec3 value)
{
	setUniformVec3(name, &value[0]);
}

void ShaderProgram::setUniformVec3(const char* name, const float * value)
{
	glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, value);

#ifdef ULTRA_DEBUG_MODE
	std::cout << "Assigning Vec3 " << name << " with the value : " << vec3ToStr(value) << std::endl;
#endif
}

void ShaderProgram::setUniformMat4(const char * name, glm::mat4 value)
{
	setUniformMat4(name, &value[0][0]);
}

void ShaderProgram::setUniformMat4(const char * name, const float * value)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, value);

#ifdef ULTRA_DEBUG_MODE
	std::cout << "Assigning Integer " << name << " with the value : " << mat4ToStr(value) << std::endl;
#endif
}

void ShaderProgram::compileShader(const char *path, uint32_t shader)
{
	std::ifstream input(path);

	std::string source;
	std::string line;

	if (input.is_open()) {
		while (std::getline(input, line)) {
			source += line + '\n';
		}
	}
	else {
		std::string errorMessage("Shader file " + *path);
		errorMessage += " not found\n";
		throw std::runtime_error(errorMessage);
	}

	const char* tempContents = source.c_str();
	glShaderSource(shader, 1, &tempContents, nullptr);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::string errorMessage("Shader compilation failed " + *path);
		errorMessage += '\n';
		errorMessage += infoLog;
		throw std::runtime_error(errorMessage);
	}
}
