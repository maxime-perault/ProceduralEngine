#include "fontShader.h"
#include <iostream>

fontShader::fontShader()
	: AShader("shaders/fontVertex.txt", "shaders/fontFragment.txt")
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

fontShader::~fontShader() {}

void	fontShader::bindAttributes(void)
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}

void	fontShader::getAllUniformLocations(void)
{
	_location_translation = getUniformLocation("translation");
	_location_colour = getUniformLocation("colour");
}

void	fontShader::loadColour(const glm::vec3 colour)
{
	this->loadVector(_location_colour, colour);
}

void	fontShader::loadPos(const glm::vec2 pos)
{
	this->loadVector(_location_translation, pos);
}