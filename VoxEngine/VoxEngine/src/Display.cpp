
#include "Display.h"

Display::Display(std::size_t win_x, std::size_t win_y)
{
	_win_x = win_x;
	_win_y = win_y;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cout << "Failed to init SDL\n" << std::endl;

	_win = SDL_CreateWindow(
		"VoxEngine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_win_x,
		_win_y,
		SDL_WINDOW_OPENGL
	);
	if (!_win)
		std::cout << "Unable to create window\n" << std::endl;

	_rend = SDL_GL_CreateContext(_win);

	SDL_GetCurrentDisplayMode(0, &_mode);

	SDL_SetWindowGrab(_win, SDL_TRUE);
	SDL_WarpMouseInWindow(_win, _win_x / 2, _win_y / 2);

	glewExperimental = GL_TRUE;
	glewInit();
}

Display::~Display()
{
	SDL_GL_DeleteContext(_rend);
	SDL_DestroyWindow(_win);
	SDL_Quit();
}

void	Display::clear(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
}

void	Display::update(void)
{
	glFlush();
	SDL_GL_SwapWindow(_win);
}

