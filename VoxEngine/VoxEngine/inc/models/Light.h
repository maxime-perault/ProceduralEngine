#ifndef LIGHT_H_
# define LIGHT_H_

# include "VAOLoader.h"
# include "Entity.h"
# include "cubeFactory.h"

class	Light
{
public:
	Light();
	Light(Entity &entity, glm::vec3 colour, float damper, float ambientLevel);
	~Light();

	Entity		_entity;
	glm::vec3	_colour;
	float		_damper;
	float		_ambientLevel;
};

#endif //!LIGHT_H_
