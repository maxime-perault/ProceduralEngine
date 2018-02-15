
#include "Light.h"

Light::Light() {}

Light::Light(Entity &entity, glm::vec3 colour, float damper, float ambientLevel)
{
	_entity = entity;
	_colour = colour;
	_damper = damper;
	_ambientLevel = ambientLevel;
	_entity._scale = glm::vec3(5, 5, 5);
}

Light::~Light()
{

}