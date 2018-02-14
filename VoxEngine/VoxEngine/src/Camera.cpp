#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <SDL2/SDL.h>

Camera::Camera(const glm::vec3 pos)
{
	_viewMatrix = glm::mat4(1.0);
	_lookAt = glm::vec3(pos.x, pos.y, pos.z + 1);
	_pos = glm::vec3(pos.x, pos.y + 1.5, pos.z);
	this->update();
	_tps = false;
}

Camera::~Camera() {}

void	Camera::update(void)
{
	_dir = glm::normalize(_lookAt - _pos);
	_rightVec = glm::normalize(glm::cross(_dir, glm::normalize(glm::vec3(0, 1, 0))));
	_UpVec = glm::cross(_rightVec, _dir);
}

void	Camera::changeView(glm::vec3 playerPos)
{
	if (_tps == false)
	{
		_lookAt = playerPos + glm::vec3(0, 2, 0);
		_pos = _pos - glm::vec3(_dir.x * 10, 0, _dir.z * 10) + glm::vec3(0, 5, 0);
	}
	else
	{
		_pos = _lookAt;
		_lookAt = glm::vec3(_dir.x, 0, _dir.z);

	}
	_tps = !_tps;
	this->update();
}

void	Camera::rotate(const glm::vec3 axe, float euler)
{
	_lookAt -= _pos;

	if (axe == glm::vec3(1, 0, 0))
			_lookAt = glm::rotate(_lookAt, (float)(euler * M_PI / 180), _rightVec);
	else if (axe == glm::vec3(0, 1, 0))
		_lookAt = glm::rotate(_lookAt, (float)(euler * M_PI / 180), _UpVec);
	else if (axe == glm::vec3(0, 0, 1))
		_lookAt = glm::rotate(_lookAt, (float)(euler * M_PI / 180), _dir);

	_lookAt += _pos;

	this->update();
}

void	Camera::rotateAround(const glm::vec3 axe, float euler)
{
	_pos -= _lookAt;

	if (axe == glm::vec3(1, 0, 0))
		_pos = glm::rotate(_pos, (float)(euler * M_PI / 180), _rightVec);
	else if (axe == glm::vec3(0, 1, 0))
		_pos = glm::rotate(_pos, (float)(euler * M_PI / 180), _UpVec);
	else if (axe == glm::vec3(0, 0, 1))
		_pos = glm::rotate(_pos, (float)(euler * M_PI / 180), _dir);

	_pos += _lookAt;

	this->update();
}

void	Camera::rotateIG(const glm::vec3 axe, float euler)
{
	if (_tps == true)
		rotateAround(axe, euler);
	else
		rotate(axe, euler);
}

void	Camera::move(const glm::vec3 delta)
{
	_pos += delta;
	_lookAt += delta;

	this->update();
}

glm::mat4	Camera::getViewMatrix()
{
	this->update();
	return (glm::lookAt(_pos, _lookAt + _dir, _UpVec));
}