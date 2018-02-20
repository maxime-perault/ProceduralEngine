#include "Game.h"
#include <iostream>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

Game::Game()
{
	_win_x = 1280; _win_y = 720;
	_display = new Display(_win_x, _win_y);
	_renderEngine = new renderEngine(_win_x, _win_y);
	_world = new World();
	_world->setWin(glm::vec2(_win_x, _win_y));
	_camera = Camera(_world->getPlayer()._pos);
	_camera.changeView(_world->getPlayer()._pos);

	_debug = true;
}

Game::~Game()
{
	delete(_world);
	delete(_renderEngine);
	delete(_display);
}

void	Game::movePlayer(const glm::vec3 delta)
{
	glm::vec3&	apos(_world->getPlayer()._pos);
	glm::vec3	next_pos(apos + delta);

	
	if (next_pos.x > apos.x)
		next_pos.x += 1.f;
	if (next_pos.y > apos.y)
		next_pos.y += 1.f;
	if (next_pos.z > apos.z)
		next_pos.z += 1.f;
	
	if (_world->getBlockOnChunk(next_pos) <= cubeFactory::WATER)
	{
		_camera.move(delta);
		apos += delta;
	}
	else
	{
		glm::vec3 deltaX(delta.x, 0, 0);
		glm::vec3 deltaY(0, delta.y, 0);
		glm::vec3 deltaZ(0, 0, delta.z);

		if (_world->getBlockOnChunk(glm::vec3(next_pos.x, apos.y, apos.z)) <= cubeFactory::WATER)
		{
			_camera.move(deltaX);
			apos += deltaX;
		}
		if (_world->getBlockOnChunk(glm::vec3(apos.x, next_pos.y, apos.z)) <= cubeFactory::WATER)
		{
			_camera.move(deltaY);
			apos += deltaY;
		}
		if (_world->getBlockOnChunk(glm::vec3(apos.x, apos.y, next_pos.z)) <= cubeFactory::WATER)
		{
			_camera.move(deltaZ);
			apos += deltaZ;
		}
	}
	_world->getPlayer().setModelMatrix();
}

void	Game::moveKeyboardCamera(float elapsed)
{
	float	speedMove = elapsed * 20;

	if (_inputManager.getRawKey(InputManager::Q))
		this->movePlayer(-glm::vec3(_camera._rightVec.x, 0, _camera._rightVec.z) * speedMove);
	if (_inputManager.getRawKey(InputManager::D))
		this->movePlayer(glm::vec3(_camera._rightVec.x, 0, _camera._rightVec.z) * speedMove);

	if (_inputManager.getRawKey(InputManager::Z))
		this->movePlayer(glm::vec3(_camera._dir.x, 0, _camera._dir.z) * speedMove);
	if (_inputManager.getRawKey(InputManager::S))
		this->movePlayer(-glm::vec3(_camera._dir.x, 0, _camera._dir.z) * speedMove);

	if (_inputManager.getRawKey(InputManager::SPACE))
		this->movePlayer(glm::vec3(0, 1, 0) * speedMove);
	if (_inputManager.getRawKey(InputManager::LCTRL))
		this->movePlayer(-glm::vec3(0, 1, 0) * speedMove);

	if (_inputManager.getKeyDown(InputManager::LALT))
		_camera.changeView(_world->getPlayer()._pos);

	if (_inputManager.getKeyDown(InputManager::TAB))
		_debug = !_debug;
	
}

void	Game::moveMouseCamera(float elapsed)
{
	float					speed = elapsed * 3000;
	static glm::vec2		pos;
	static glm::vec2		delta;
	static const glm::vec2	lastPos(_display->_mode.w / 2, _display->_mode.h / 2);

	pos = _inputManager.getMouseMotion(_display->_win, _win_x, _win_y);

	if (pos.x == -1 && pos.y == -1)
		return;

	delta = pos - lastPos;

	_camera.rotateIG(glm::vec3(0, 1, 0), -(float)delta.x * (elapsed * 50.f));
	_camera.rotateIG(glm::vec3(1, 0, 0), -(float)delta.y * (elapsed * 50.f));

	_world->getPlayer()._rot.y = -atan2(_camera._dir.z, _camera._dir.x);
	_world->getPlayer().setModelMatrix();
}

void Game::loop(void)
{
	float	elapsed, totalTime = 0;
	int		frame = 0;
	int		fps = -1;

	elapsed = _timer.getElapsedSeconds(true);
	while (true)
	{
		_inputManager.Update();

		if (_inputManager.getEvent(SDL_QUIT) || _inputManager.getKeyDown(InputManager::ESCAPE))
			return;
		
		elapsed = _timer.getElapsedSeconds(true);
		this->moveMouseCamera(elapsed);
		this->moveKeyboardCamera(elapsed);
		
		_display->clear();
		_world->update(elapsed, _camera, fps, _debug);
		_renderEngine->renderWorld(_camera, _world, _debug);
		_display->update();

		++frame;
		totalTime += elapsed;
		if (totalTime >= 1)
		{
			fps = frame;
			frame = 0;
			totalTime = 0;
		}
	}
}