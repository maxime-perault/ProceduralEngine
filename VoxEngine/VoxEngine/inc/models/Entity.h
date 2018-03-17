
#ifndef ENTITY_HH_
# define ENTITY_HH_

# include "texturedModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>

class World;

class	Entity
{
private:
	glm::mat4	_modelMatrix;

	void	translate(glm::vec3 pos);
	void	rotate(glm::vec3 axe, float euler);
	void	scale(glm::vec3 scal);
public:
	Entity(texturedModel& model);
	Entity();
	~Entity();

	texturedModel	_model;
	glm::vec3		_pos;
	glm::vec3		_rot;
	glm::vec3		_scale;

	glm::vec4		_colour;

	bool			_jump;
	float			_v0;
	float			_y0;
	float			_g;
	float			_t;

	bool			_draw;

	void			setModelMatrix(void);
	glm::mat4		getModelMatrix(void);

	glm::vec3&		getPos(void);

	glm::vec3	move(const glm::vec3 delta, float elapsed, World *world);
	void		jump(float h);
	bool		isFalling(World *world);
	bool		canMove(World *world);
};

#endif //!ENTITY_HH_
