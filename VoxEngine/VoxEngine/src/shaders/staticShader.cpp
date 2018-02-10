#include "staticShader.h"


staticShader::staticShader()
	: AShader(VERTEX_FILE, FRAGMENT_FILE)
{
}

staticShader::~staticShader() {}

void	staticShader::bindAttributes(void)
{
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoords");
}
