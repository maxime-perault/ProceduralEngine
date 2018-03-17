#include "AShader.h"
#include <string>
#include <iostream>
#include <fstream>

AShader::AShader(const std::string vertexFile, const std::string fragmentFile)
{
	std::cout << "[SHADER] " << vertexFile << std::endl;
	_vertex_shader_id = this->loadShader(vertexFile, GL_VERTEX_SHADER);
	std::cout << "[SHADER] " << fragmentFile << std::endl;
	_fragment_shader_id = this->loadShader(fragmentFile, GL_FRAGMENT_SHADER);
}

AShader::~AShader() {}

int	AShader::getUniformLocation(const GLchar *uniformName) const
{
	return (glGetUniformLocation(_program_id, uniformName));
}

void	AShader::start(void)
{
	glUseProgram(_program_id);
}

void	AShader::stop(void)
{
	glUseProgram(0);
}

void	AShader::cleanUp(void)
{
	this->stop();
	glDetachShader(_program_id, _vertex_shader_id);
	glDetachShader(_program_id, _fragment_shader_id);
}

void	AShader::bindAttribute(const int attribute,
	const GLchar *variableName)
{
	std::cout << "[VAR] " << variableName << " -> OK" << std::endl;
	glBindAttribLocation(_program_id, attribute, variableName);
}

void	AShader::loadMatrix(const int location, const glm::mat4 m) const
{
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(m));
}

void	AShader::loadBool(const int location, const bool value) const
{
	float tmp(0);

	if (value)
		tmp = 1;
	glUniform1f(location, tmp);
}

void	AShader::loadVector(const int location, const glm::vec4 v) const
{
	glUniform4fv(location, 1, glm::value_ptr(v));
}

void	AShader::loadVector(const int location, const glm::vec3 v) const
{
	glUniform3fv(location, 1, glm::value_ptr(v));
}

void	AShader::loadVector(const int location, const glm::vec2 v) const
{
	glUniform2fv(location, 1, glm::value_ptr(v));
}

void	AShader::loadFloat(const int location, const float value) const
{
	glUniform1f(location, value);
}

int		AShader::loadShader(const std::string file, const int type)
{
	std::ifstream	stream(file.c_str());
	std::string		line, txt;

	if (!stream)
		std::cout << std::string("could not read \"" + file + "\"") << std::endl;

	while (getline(stream, line))
		txt.append(line).append("\n");

	stream.close();

	int	shaderID = glCreateShader(type);

	if (shaderID == 0)
		std::cout << "could not create the shader" << std::endl;

	const GLchar	*c_str = txt.c_str();
	GLint			code_size = txt.size();

	glShaderSource(shaderID, 1, &c_str, (GLint*)&code_size);
	glCompileShader(shaderID);

	GLint	ret;

	this->CheckShader(shaderID, GL_COMPILE_STATUS, &ret,
		"unable to compile shader");

	return (shaderID);
}

/*
** Fonction de check des shaders et du program en fonction de l'id
*/
void	AShader::CheckShader(GLuint id, GLuint type, GLint *ret, const char *onfail)
{
	switch (type)
	{
	case(GL_COMPILE_STATUS):
	{
		glGetShaderiv(id, type, ret);
		if (*ret == false)
		{

			int infologLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLength);

			GLchar *buffer = new GLchar[infologLength];
			GLsizei charsWritten = 0;

			std::cout << onfail << std::endl;

			glGetShaderInfoLog(id, infologLength, &charsWritten, buffer);
			std::cout << buffer << std::endl;
			delete buffer;
		}
		break;
	}
	case(GL_LINK_STATUS):
	{
		glGetProgramiv(id, type, ret);
		if (*ret == false)
		{
			int infologLength = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infologLength);

			GLchar *buffer = new GLchar[infologLength];
			GLsizei charsWritten = 0;

			std::cout << onfail << std::endl;

			glGetProgramInfoLog(id, infologLength, &charsWritten, buffer);
			std::cout << buffer << std::endl;
			delete buffer;
		}
		break;
	}
	default:
		break;
	};
}
