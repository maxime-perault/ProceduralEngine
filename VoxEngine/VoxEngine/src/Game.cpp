#include "Game.h"
#include <iostream>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

Game::Game()
{
	_win_x = 1280; _win_y = 720;
	_display = new Display(_win_x, _win_y);
	_renderEngine = new renderEngine(_win_x, _win_y);
	_inputManager = new InputManager();
	_world = new World(_win_x, _win_y);
	_camera = new Camera(_world->getPlayer()->_pos);
	_timer = new Timer();

	_debug = true;
}

Game::~Game()
{
	//DELETEs
}

void	Game::movePlayer(const glm::vec3 delta)
{
	_camera->move(delta);
	_world->getPlayer()->_pos += delta;
	
}

void	Game::moveKeyboardCamera(float elapsed)
{
	float	speedMove = elapsed * 20;

	if (_inputManager->getRawKey(InputManager::Q))
		this->movePlayer(-glm::vec3(_camera->_rightVec.x, 0, _camera->_rightVec.z) * speedMove);
	if (_inputManager->getRawKey(InputManager::D))
		this->movePlayer(glm::vec3(_camera->_rightVec.x, 0, _camera->_rightVec.z) * speedMove);

	if (_inputManager->getRawKey(InputManager::Z))
		this->movePlayer(glm::vec3(_camera->_dir.x, 0, _camera->_dir.z) * speedMove);
	if (_inputManager->getRawKey(InputManager::S))
		this->movePlayer(-glm::vec3(_camera->_dir.x, 0, _camera->_dir.z) * speedMove);

	if (_inputManager->getRawKey(InputManager::SPACE))
		this->movePlayer(glm::vec3(0, 1, 0) * speedMove);
	if (_inputManager->getRawKey(InputManager::LCTRL))
		this->movePlayer(-glm::vec3(0, 1, 0) * speedMove);

	if (_inputManager->getKeyDown(InputManager::LALT))
		_camera->changeView(_world->getPlayer()->_pos);

	if (_inputManager->getKeyDown(InputManager::TAB))
		_debug = !_debug;
	
}

void	Game::moveMouseCamera(float elapsed)
{
	float					speed = elapsed * 3000;
	static glm::vec2		pos;
	static glm::vec2		delta;
	static const glm::vec2	lastPos(_display->_mode.w / 2, _display->_mode.h / 2);

	pos = _inputManager->getMouseMotion(_display->_win, _win_x, _win_y);

	if (pos.x == -1 && pos.y == -1)
		return;

	delta = pos - lastPos;

	_camera->rotateIG(glm::vec3(0, 1, 0), -(float)delta.x * (elapsed * 300.f));
	_camera->rotateIG(glm::vec3(1, 0, 0), -(float)delta.y * (elapsed * 300.f));
}

void Game::loop(void)
{
	float	elapsed, totalTime = 0;
	int		frame = 0;
	int		fps = -1;

	while (true)
	{
		_inputManager->Update();

		if (_inputManager->getEvent(SDL_QUIT) || _inputManager->getKeyDown(InputManager::ESCAPE))
			return;
		
		elapsed = _timer->getElapsedSeconds(true);
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