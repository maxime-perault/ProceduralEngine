
#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>

Entity::Entity() {}

Entity::Entity(texturedModel& model)
{
	_model = model;
	_modelMatrix = glm::mat4(1.0);
	_pos = glm::vec3(0, 0, 0);
	_scale = glm::vec3(1, 1, 1);
	_rot = glm::vec3(0, 0, 0);
	_colour = glm::vec3(1, 1, 1);
	_draw = true;
}

Entity::~Entity() {}

void	Entity::translate(glm::vec3 pos)
{
	_modelMatrix = glm::translate(_modelMatrix, pos);
}
void	Entity::rotate(glm::vec3 axe, float euler)
{
	_modelMatrix = glm::rotate(_modelMatrix, euler, axe);
}

void	Entity::scale(glm::vec3 scal)
{
	_modelMatrix = glm::scale(_modelMatrix, scal);
}

glm::vec3&		Entity::getPos(void)
{
	return _pos;
}

glm::mat4	Entity::getModelMatrix(void)
{
	_modelMatrix = glm::mat4(1.0);

	this->translate(_pos);
	this->rotate(glm::vec3(1, 0, 0), _rot.x);
	this->rotate(glm::vec3(0, 1, 0), _rot.y);
	this->rotate(glm::vec3(0, 0, 1), _rot.z);
	this->scale(_scale);

	return(_modelMatrix);
}