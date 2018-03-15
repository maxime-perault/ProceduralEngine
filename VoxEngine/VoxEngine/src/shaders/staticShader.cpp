#include "staticShader.h"
#include <iostream>


staticShader::staticShader()
	: AShader("shaders/vertexShader.txt", "shaders/fragmentShader.txt")
{
	if ((_program_id = glCreateProgram()) == 0)
		std::cout << "could not create the shader program" << std::endl;

	glAttachShader(_program_id, _vertex_shader_id);
	glAttachShader(_program_id, _fragment_shader_id);

	this->bindAttributes();
	glLinkProgram(_program_id);

	GLint ret;

	this->CheckShader(_program_id, GL_LINK_STATUS, &ret, "unable to link the program!");

	glValidateProgram(_program_id);
	this->getAllUniformLocations();
}

staticShader::~staticShader() {}

void	staticShader::bindAttributes(void)
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
	this->bindAttribute(2, "normal");
}

void		staticShader::getAllUniformLocations(void)
{
	_location_transformation_matrix = getUniformLocation("transformationMatrix");
	_location_projection_matrix = getUniformLocation("projectionMatrix");
	_location_view_matrix = getUniformLocation("viewMatrix");
	_location_shadow_matrix = getUniformLocation("shadowMapSpace");

	_location_Atlas = getUniformLocation("textureSampler");
	_location_shadowMap = getUniformLocation("shadowMap");

	_location_lightPos = getUniformLocation("lightPosition");
	_location_lightColour = getUniformLocation("lightColour");
	_location_shineDamper = getUniformLocation("shineDamper");
	_location_ambientLevel = getUniformLocation("AmbientLevel");

	_location_rawColour = getUniformLocation("rawColour");
	_location_colour = getUniformLocation("colour");
}

void	staticShader::attTextures(void)
{
	glUniform1i(_location_Atlas, 0);
	glUniform1i(_location_shadowMap, 1);
}


void	staticShader::loadTransformationMatrix(const glm::mat4 m) const
{
	/*
	std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << ", " << std::endl;
	std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << ", " << std::endl;
	std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << ", " << std::endl;
	std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ", " << std::endl;
	*/
	this->loadMatrix(_location_transformation_matrix, m);
}

void	staticShader::loadProjectionMatrix(const glm::mat4 m) const
{
	/*
	std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << ", " << std::endl;
	std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << ", " << std::endl;
	std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << ", " << std::endl;
	std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ", " << std::endl;
	*/
	this->loadMatrix(_location_projection_matrix, m);
}

void	staticShader::loadViewMatrix(const glm::mat4 m) const
{
	/*
	std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << ", " << std::endl;
	std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << ", " << std::endl;
	std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << ", " << std::endl;
	std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ", " << std::endl;
	*/
	this->loadMatrix(_location_view_matrix, m);
}

void	staticShader::loadShadowMatrix(const glm::mat4 m) const
{
	/*
	std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << ", " << std::endl;
	std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << ", " << std::endl;
	std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << ", " << std::endl;
	std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ", " << std::endl;
	*/
	this->loadMatrix(_location_shadow_matrix, m);
}

void	staticShader::loadLight(const glm::vec3 pos, const glm::vec3 colour, const float damper, const float ambientLevel) const
{
	this->loadVector(_location_lightPos, pos);
	this->loadVector(_location_lightColour, colour);
	this->loadFloat(_location_shineDamper, damper);
	this->loadFloat(_location_ambientLevel, ambientLevel);
}

void	staticShader::loadRawColour(glm::vec3(colour)) const
{
	this->loadBool(_location_rawColour, true);
	this->loadVector(_location_colour, colour);
}

void	staticShader::unloadRawColour(void) const
{
	this->loadBool(_location_rawColour, false);
}