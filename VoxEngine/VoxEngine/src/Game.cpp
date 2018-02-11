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
	_camera = new Camera(glm::vec3(0, 0, -5));
}

Game::~Game()
{
	delete(_renderEngine);
	delete(_camera);
	delete(_inputManager);
	delete(_display);
}

double	Game::getElapsedTime(void)
{
	static Uint32 last_time = SDL_GetTicks();

	last_time = SDL_GetTicks() - last_time;
	return ((double)last_time / 10000);
}

void	Game::moveCamera(float elapsed)
{
	//////////// KEYBOARD-EVENT //////////
	float	speedMove = elapsed / 4;

	if (_inputManager->getRawKey(SDL_SCANCODE_A))
		_camera->move(-glm::vec3(_camera->_rightVec.x, 0, _camera->_rightVec.z) * speedMove);
	if (_inputManager->getRawKey(SDL_SCANCODE_D))
		_camera->move(glm::vec3(_camera->_rightVec.x, 0, _camera->_rightVec.z) * speedMove);

	if (_inputManager->getRawKey(SDL_SCANCODE_W))
		_camera->move(glm::vec3(_camera->_dir.x, 0, _camera->_dir.z) * speedMove);
	if (_inputManager->getRawKey(SDL_SCANCODE_S))
		_camera->move(-glm::vec3(_camera->_dir.x, 0, _camera->_dir.z) * speedMove);

	if (_inputManager->getRawKey(SDL_SCANCODE_SPACE))
		_camera->move(glm::vec3(0, 1, 0) * speedMove);
	if (_inputManager->getRawKey(SDL_SCANCODE_LCTRL))
		_camera->move(-glm::vec3(0, 1, 0) * speedMove);

	//////////// MOUSE-EVENT ////////////
	static glm::vec2 pos;
	float speed = elapsed * 300, deltaX = 1, deltaY = 1;

	pos = _inputManager->getMouseMotion(_display->_win, _win_x, _win_y);

	if (pos.x == -1 && pos.y == -1)
		return;

	deltaX = pos.x - _display->_mode.w / 2;
	if (deltaX > speed)
		deltaX = speed;
	else if (deltaX < -speed)
		deltaX = -speed;

	deltaY = pos.y - _display->_mode.h / 2;
	if (deltaY > speed)
		deltaY = speed;
	else if (deltaY < -speed)
		deltaY = -speed;

	if (deltaX != 0)
		_camera->rotate(glm::vec3(0, 1, 0), -deltaX);
	
	if (deltaY != 0)
		_camera->rotate(glm::vec3(1, 0, 0), -deltaY);
}

void Game::loop(void)
{
	float	elapsed;

	while (true)
	{
		_inputManager->Update();

		if (_inputManager->getEvent(SDL_QUIT) || _inputManager->getKeyDown(SDLK_ESCAPE))
			return;
		
		elapsed = this->getElapsedTime();
		this->moveCamera(elapsed);
		
		_display->clear();
		_renderEngine->renderEntities(_camera);
		_display->update();
	}
}