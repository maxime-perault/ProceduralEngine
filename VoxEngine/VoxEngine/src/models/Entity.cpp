
#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>
#include "world.h"

Entity::Entity() {}

Entity::Entity(texturedModel& model)
{
	_model = model;
	_modelMatrix = glm::mat4(1.0);
	_pos = glm::vec3(0, 0, 0);
	_scale = glm::vec3(1, 1, 1);
	_rot = glm::vec3(0, 0, 0);
	_colour = glm::vec4(1, 1, 1, 1);
	_draw = true;
	_jump = false;
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

void	Entity::setModelMatrix(void)
{
	_modelMatrix = glm::mat4(1.0);

	this->translate(_pos);
	this->rotate(glm::vec3(1, 0, 0), _rot.x);
	this->rotate(glm::vec3(0, 1, 0), _rot.y);
	this->rotate(glm::vec3(0, 0, 1), _rot.z);
	this->scale(_scale);
}

glm::mat4	Entity::getModelMatrix(void)
{
	return(_modelMatrix);
}

bool		Entity::canMove(World *world)
{
	if (world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y + 1.f, _pos.z + 1.f)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y + 1.f, _pos.z + 1.f)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y, _pos.z + 1.f)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y, _pos.z + 1.f)) > cubeFactory::WATER ||

		world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y + 1.f, _pos.z)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y + 1.f, _pos.z)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y, _pos.z)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y, _pos.z)) > cubeFactory::WATER)
	{
		return false;
	}

	if (world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y + 1.f, _pos.z + 1.f)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y + 1.f, _pos.z + 1.f)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y, _pos.z + 1.f)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y, _pos.z + 1.f)) > cubeFactory::WATER ||

		world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y + 1.f, _pos.z)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y + 1.f, _pos.z)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y, _pos.z)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y, _pos.z)) > cubeFactory::WATER)
	{
		return false;
	}

	if (world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y + 1.f, _pos.z + 1.f)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y + 1.f, _pos.z + 1.f)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y, _pos.z + 1.f)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y, _pos.z + 1.f)) > cubeFactory::WATER ||

		world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y + 1.f, _pos.z)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y + 1.f, _pos.z)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x + 1.f, _pos.y, _pos.z)) > cubeFactory::WATER ||
		world->getBlockOnChunk(glm::vec3(_pos.x, _pos.y, _pos.z)) > cubeFactory::WATER)
	{
		return false;
	}
	return true;
}

glm::vec3	Entity::move(const glm::vec3 delta, float elapsed, World *world)
{
	glm::vec3&	apos(_pos);
	glm::vec3	next_pos(apos + delta);
	glm::vec3	res(0, 0, 0);

	glm::vec3	deltaX(delta.x, 0, 0);
	glm::vec3	deltaY(0, delta.y, 0);
	glm::vec3	deltaZ(0, 0, delta.z);

	static float nextY;

	_t += elapsed;

	// f(t) = 1/2 * g * t^2 + v0 * t + p0
	if (_jump == true)
	{
		nextY = (0.5f * _g * (_t * _t)) + (_v0 * _t) + _y0;

		deltaY = glm::vec3(0, nextY - apos.y, 0);
		next_pos.y = nextY;

		if (nextY == 0)
			_jump = false;
	}

	if (world->getBlockOnChunk(glm::vec3(next_pos.x + 1.f,	apos.y + 1.f,	apos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(next_pos.x,		apos.y + 1.f,	apos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(next_pos.x + 1.f,	apos.y,			apos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(next_pos.x,		apos.y,			apos.z + 1.f)) <= cubeFactory::WATER &&

		world->getBlockOnChunk(glm::vec3(next_pos.x + 1.f,	apos.y + 1.f,	apos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(next_pos.x,		apos.y + 1.f,	apos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(next_pos.x + 1.f,	apos.y,			apos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(next_pos.x,		apos.y,			apos.z)) <= cubeFactory::WATER)
	{
		apos += deltaX;
		res += deltaX;
	}

	if (world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	next_pos.y + 1.f,	apos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		next_pos.y + 1.f,	apos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	next_pos.y,			apos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		next_pos.y,			apos.z + 1.f)) <= cubeFactory::WATER &&

		world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	next_pos.y + 1.f,	apos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		next_pos.y + 1.f,	apos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	next_pos.y,			apos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		next_pos.y,			apos.z)) <= cubeFactory::WATER)
	{
		apos += deltaY;
		res += deltaY;
	}
	else if (_jump == true)
	{
		
		deltaY =  glm::vec3(0, (float)((int)apos.y) - apos.y, 0);
		apos += deltaY;
		res += deltaY;
		_jump = false;
	}

	if (world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	apos.y + 1.f,	next_pos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		apos.y + 1.f,	next_pos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	apos.y,			next_pos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		apos.y,			next_pos.z + 1.f)) <= cubeFactory::WATER &&

		world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	apos.y + 1.f,	next_pos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		apos.y + 1.f,	next_pos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	apos.y,			next_pos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		apos.y,			next_pos.z)) <= cubeFactory::WATER)
	{
		apos += deltaZ;
		res += deltaZ;
	}
	this->setModelMatrix();
	return (res);
}

void	Entity::jump(float h)
{
	static const float th = 0.25f;

	if (h == 0)
	{
		_y0 = _pos.y - 1.f;
		h = 1;
		_t = th;
	}
	else
	{
		_y0 = _pos.y;
		_t = 0;
	}

	_jump = true;
	_v0 = (2.f * h) / th;
	_g = (-2.f * h) / (th * th);
}

bool	Entity::isFalling(World *world)
{
	glm::vec3&	apos(_pos);

	if (world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	apos.y - 1.f + 1.f, apos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		apos.y - 1.f + 1.f, apos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	apos.y - 1.f,		apos.z + 1.f)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		apos.y - 1.f,		apos.z + 1.f)) <= cubeFactory::WATER &&

		world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	apos.y - 1.f + 1.f, apos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		apos.y - 1.f + 1.f, apos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x + 1.f,	apos.y - 1.f,		apos.z)) <= cubeFactory::WATER &&
		world->getBlockOnChunk(glm::vec3(apos.x,		apos.y - 1.f,		apos.z)) <= cubeFactory::WATER)
	{
		return true;
	}
	return false;
}