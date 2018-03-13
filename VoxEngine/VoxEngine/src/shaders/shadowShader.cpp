#include "shadowShader.h"
#include <iostream>

shadowShader::shadowShader()
	: AShader("shaders/shadowVertex.txt", "shaders/shadowFragment.txt")
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

shadowShader::~shadowShader() {}

void	shadowShader::bindAttributes(void)
{
	this->bindAttribute(0, "position");
}

void	shadowShader::getAllUniformLocations(void)
{
	_location_depthMVP = getUniformLocation("depthMVP");
}

void	shadowShader::loadMVP(const glm::mat4 mvp)
{
	this->loadMatrix(_location_depthMVP, mvp);
}
