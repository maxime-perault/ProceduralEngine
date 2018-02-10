
#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>

Entity::Entity(texturedModel *model)
{
	_model = model;
	_modelMatrix = glm::mat4(1.0);
}

Entity::~Entity() {}

void	Entity::translate(glm::vec3 pos)
{
	_modelMatrix = glm::translate(_modelMatrix, pos);
}
void	Entity::rotation(glm::vec3 axe, float euler)
{
	_modelMatrix = glm::rotate(_modelMatrix, euler, axe);
}

void	Entity::scale(glm::vec3 scal)
{
	_modelMatrix = glm::scale(_modelMatrix, scal);
}