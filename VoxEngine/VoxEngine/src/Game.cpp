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
	_timer = new Timer();
}

Game::~Game()
{
	//DELETE
}

void	Game::moveKeyboardCamera(float elapsed)
{
	float	speedMove = elapsed * 20;

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

	_camera->rotate(glm::vec3(0, 1, 0), -(float)delta.x * (elapsed * 300.f));
	_camera->rotate(glm::vec3(1, 0, 0), -(float)delta.y * (elapsed * 300.f));
}

void Game::loop(void)
{
	float	elapsed, totalTime = 0;
	int		frame = 0;

	while (true)
	{
		_inputManager->Update();

		if (_inputManager->getEvent(SDL_QUIT) || _inputManager->getKeyDown(SDLK_ESCAPE))
			return;
		
		elapsed = _timer->getElapsedSeconds(true);
		this->moveMouseCamera(elapsed);
		this->moveKeyboardCamera(elapsed);
		
		_display->clear();
		_renderEngine->renderEntities(_camera);
		_display->update();

		++frame;
		totalTime += elapsed;
		if (totalTime >= 1)
		{
			std::cout << frame << " FPS." << std::endl;
			frame = 0;
			totalTime = 0;
		}
	}
}