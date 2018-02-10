#ifndef DISPLAY_HH_
# define DISPLAY_HH_

#include <iostream>
#include <gl/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include "renderEngine.h"

class	Display
{
public:
	SDL_Window		*_win;
	SDL_GLContext	_rend;

	std::size_t	_win_y;
	std::size_t	_win_x;

	renderEngine *_renderEngine;

	Display();
	~Display();

	void	clear(void);
	void	update(void);
	void	all(void);
};

#endif // !DISPLAY_HH_ 

