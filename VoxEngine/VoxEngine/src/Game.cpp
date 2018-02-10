#include "Game.h"
#include <iostream>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

Game::Game()
{
	_display = new Display();
}

Game::~Game()
{
	delete(_display);
}

void Game::loop(void)
{
	const Uint8	*keystates;
	SDL_Event	event;

	/*
	** GAME LOOP
	*/
	while (true)
	{
		if (SDL_PollEvent(&event))
		{
			keystates = SDL_GetKeyboardState(NULL);
			if (event.type == SDL_QUIT || keystates[SDL_SCANCODE_ESCAPE])
			{
				return;
			}
		}
		_display->clear();
		_display->all();
		_display->update();
	}
}