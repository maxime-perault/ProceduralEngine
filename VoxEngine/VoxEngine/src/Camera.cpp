#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>

Camera::Camera(const glm::vec3 pos)
{
	_viewMatrix = glm::mat4(1.0);
	this->translate(pos);
}

Camera::~Camera() {}

void	Camera::rotate(const glm::vec3 axe, float euler)
{
	_viewMatrix = glm::rotate(_viewMatrix, (float)(M_PI * euler / 180), axe);
}

void	Camera::translate(const glm::vec3 pos)
{
	_pos = pos;
	_viewMatrix = glm::translate(_viewMatrix, pos);
}