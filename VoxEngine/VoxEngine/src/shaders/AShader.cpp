#include "AShader.h"
#include <string>
#include <iostream>
#include <fstream>

AShader::AShader(const std::string &vertexFile, const std::string &fragmentFile)
{
	_vertex_shader_id = this->loadShader(vertexFile, GL_VERTEX_SHADER);
	_fragment_shader_id = this->loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	if ((_program_id = glCreateProgram()) == 0)
		std::cout << "could not create the shader program" << std::endl;

	glAttachShader(_program_id, _vertex_shader_id);
	glAttachShader(_program_id, _fragment_shader_id);

	this->bindAttributes();
	glLinkProgram(_program_id);

	GLint ret;

	this->CheckShader(_program_id, GL_LINK_STATUS, &ret, "unable to link the program!");

	glValidateProgram(_program_id);
	getAllUniformLocations();
}

AShader::~AShader() {}

void		AShader::getAllUniformLocations(void)
{
	_location_transformation_matrix = getUniformLocation("transformationMatrix");
	_location_projection_matrix = getUniformLocation("projectionMatrix");
	_location_view_matrix = getUniformLocation("viewMatrix");

	_location_lightPos = getUniformLocation("lightPosition");
	_location_lightColour = getUniformLocation("lightColour");
	_location_shineDamper = getUniformLocation("shineDamper");
	_location_ambientLevel = getUniformLocation("AmbientLevel");
}

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

void	AShader::bindAttribute(const int &attribute,
	const GLchar *variableName)
{
	std::cout << "[VAR] " << variableName << " -> binded" << std::endl;
	glBindAttribLocation(_program_id, attribute, variableName);
}

void	AShader::bindAttributes(void)
{
	// Call if from herited shaders
}

void	AShader::loadMatrix(const int &location, const glm::mat4 &m) const
{
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(m));
}

void	AShader::loadBool(const int &location, const bool &value) const
{
	float tmp(0);

	if (value)
		tmp = 1;
	glUniform1f(location, tmp);
}

void	AShader::loadVector(const int &location, const glm::vec3 &v) const
{
	glUniform3fv(location, 1, glm::value_ptr(v));
}

void	AShader::loadFloat(const int &location, const float &value) const
{
	glUniform1f(location, value);
}

void	AShader::loadTransformationMatrix(const glm::mat4 &m) const
{
	/*
	std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << ", " << std::endl;
	std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << ", " << std::endl;
	std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << ", " << std::endl;
	std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ", " << std::endl;
	*/
	this->loadMatrix(_location_transformation_matrix, m);
}

void	AShader::loadProjectionMatrix(const glm::mat4 &m) const
{
	/*
	std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << ", " << std::endl;
	std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << ", " << std::endl;
	std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << ", " << std::endl;
	std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ", " << std::endl;
	*/
	this->loadMatrix(_location_projection_matrix, m);
}

void	AShader::loadViewMatrix(const glm::mat4 &m) const
{
	/*
	std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << ", " << std::endl;
	std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << ", " << std::endl;
	std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << ", " << std::endl;
	std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ", " << std::endl;
	*/
	this->loadMatrix(_location_view_matrix, m);
}

void	AShader::loadLight(const glm::vec3 pos, const glm::vec3 colour, const float damper, const float ambientLevel) const
{
	this->loadVector(_location_lightPos, pos);
	this->loadVector(_location_lightColour, colour);
	this->loadFloat(_location_shineDamper, damper);
	this->loadFloat(_location_ambientLevel, ambientLevel);
}


int		AShader::loadShader(const std::string &file, const int &type)
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
